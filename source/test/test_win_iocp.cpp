/*
===============================================================================
2023/04/07: ��ֲMSDN�ϵ�demo
2023/04/10: �ӿڷ�װ

�ο�: 
https://learn.microsoft.com/en-us/windows/win32/api/mswsock/nf-mswsock-acceptex
===============================================================================
*/

#ifndef UNICODE
#define UNICODE
#endif

#define WIN32_LEAN_AND_MEAN

#include <winsock2.h>
#include <mswsock.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdint.h>
#include <memory.h>
#include <functional>
#include <iostream>
#include <array>
#include <unordered_map>
#include "ring_buffer.h"
#include "sigslot/signal.hpp"

// Need to link with Ws2_32.lib
#pragma comment(lib, "Ws2_32.lib")

class iocp
{
public:
    int create()
    {
        assert(NULL == iocp_handle_);
        iocp_handle_ = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, (u_long)0, 0);
        return 0;
    }

    void destroy()
    {
        CloseHandle(iocp_handle_);
    }

    int bind(SOCKET socket, void* ud)
    {
        assert(NULL != iocp_handle_);
        HANDLE result = CreateIoCompletionPort((HANDLE)socket, iocp_handle_, (ULONG_PTR)ud, 0);
        assert(result == iocp_handle_);
        return 0;
    }

    size_t poll(OVERLAPPED_ENTRY* entries, int count)
    {
        //TODO:�ǲ��Ƕ��ڴ������ʲôҪ�󣬷���std::vector�����
        ULONG ulNumEntriesRemoved = 0;
        DWORD dwMillisecondsToWait = 0;
        GetQueuedCompletionStatusEx(iocp_handle_, entries, count, &ulNumEntriesRemoved, dwMillisecondsToWait, FALSE);
        return ulNumEntriesRemoved;
    }

private:
    HANDLE iocp_handle_ = NULL;
};

enum class async_io_type
{
    invalid,
    accept,
    recv,
    send,
};

struct async_context
{
    WSAOVERLAPPED overlapped;
    async_io_type type;
};

struct accept_context : public async_context
{
    accept_context()
    {
        memset(this, 0, sizeof(*this));
        type = async_io_type::accept;
    }

    SOCKET handle;
    sockaddr_in local_addr;
    uint8_t local_padding[16];
    sockaddr_in remote_addr;
    uint8_t remote_padding[16];
};

struct recv_context : public async_context
{
    recv_context()
    {
        memset(this, 0, sizeof(*this));
        type = async_io_type::recv;
    }

    size_t buf_used_count;
    WSABUF buf[2];
    class tcp_connection* connection;
};

class tcp_connection
{
    friend class tcp_server;

public:
    sigslot::signal<uint8_t*, size_t> sig_on_read_;

public:
    tcp_connection()
        : peer_socket_(INVALID_SOCKET),
        recv_buffer_(8)
    {
        is_recv_post_ = false;
    }

    /** �������� */
    int send_data(uint8_t* data, size_t sz)
    {
        //TODO:
        return 0;
    }

private:
    int attach(SOCKET handle)
    {
        assert(peer_socket_ == INVALID_SOCKET);
        peer_socket_ = handle;
        return 0;
    }

    void close()
    {
        //closesocket(peer_socket_);
    }

    int start_recv()
    {
        if (is_recv_post_) 
        {
            return -1; //�Ѿ�Ͷ����Recv����
        }

        std::vector<std::tuple<uint8_t*, size_t> > sections = recv_buffer_.get_writable_sections();
        if (sections.empty())
        {
            return -1; //���ջ���������~
        }

        recv_context_.buf_used_count = sections.size();
        for (size_t i = 0; i < sections.size(); ++i)
        {
            recv_context_.buf[i].buf = (CHAR*)std::get<0>(sections[i]);
            recv_context_.buf[i].len = std::get<1>(sections[i]);
        }
        recv_context_.connection = this;
        DWORD dwFlags = 0;
        int errorno = WSARecv(peer_socket_, recv_context_.buf, recv_context_.buf_used_count, 
                              NULL, &dwFlags, &recv_context_.overlapped, NULL);
        if (0 == errorno)
        {
            // TODO:���������
        }
        else if (SOCKET_ERROR == errorno)
        {
            if (WSA_IO_PENDING != WSAGetLastError())
            {
                // �첽������ʧ��
            }
            else
            {
                // �첽����ɹ�����
                is_recv_post_ = true;
            }
        }

        return 0;
    }

    void on_recv_data()
    {
        DWORD dwTransferBytes = 0;
        DWORD dwFlags = 0;
        WSAGetOverlappedResult(peer_socket_, &recv_context_.overlapped, &dwTransferBytes, FALSE, &dwFlags);
        recv_buffer_.advance_write_pos(dwTransferBytes);

        size_t sz = recv_buffer_.get_unread_size();
        std::unique_ptr<uint8_t[]> buffer(new uint8_t[sz]);
        recv_buffer_.read(buffer.get(), sz);
        sig_on_read_(buffer.get(), sz);

        is_recv_post_ = false;
        start_recv();

        //TODO:���dwTransferBytesΪ0��ʾ�Է��Ͽ�������
    }

private:
    SOCKET peer_socket_;
    ring_buffer recv_buffer_;
    recv_context recv_context_;
    uint32_t is_recv_post_ : 1;
};

class tcp_server
{
public:
    sigslot::signal<tcp_connection*> sig_on_connection_;

public:
    int create(uint16_t port)
    {
        if (iocp_.create())
        {
            return -1;
        }

        // ���������׽���
        listen_socket_ = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (listen_socket_ == INVALID_SOCKET)
        {
            wprintf(L"Create of ListenSocket socket failed with error: %u\n", WSAGetLastError());
            return -1;
        }

        // �׽�������ɶ˿ڰ�
        iocp_.bind(listen_socket_, (void*)kListenSockCompletionKey);

        // �����׽��ְ󶨵�����IP+�˿�
        sockaddr_in service;
        service.sin_family = AF_INET;
        service.sin_addr.s_addr = INADDR_ANY;
        service.sin_port = htons(port);
        if (bind(listen_socket_, (SOCKADDR*)&service, sizeof(service)) == SOCKET_ERROR)
        {
            wprintf(L"bind failed with error: %u\n", WSAGetLastError());
            closesocket(listen_socket_);
            return -1;
        }

        // ��ʼ����
        if (listen(listen_socket_, kMaxBackLog) == SOCKET_ERROR)
        {
            wprintf(L"listen failed with error: %u\n", WSAGetLastError());
            closesocket(listen_socket_);
            return -1;
        }

        // ��ȡMS��չ������ַ
        GUID GuidAcceptEx = WSAID_ACCEPTEX;
        DWORD dwBytesReturned = 0;
        int iResult = WSAIoctl(listen_socket_, SIO_GET_EXTENSION_FUNCTION_POINTER,
                               &GuidAcceptEx, sizeof(GuidAcceptEx),
                               &lpfnAcceptEx, sizeof(lpfnAcceptEx),
                               &dwBytesReturned, NULL, NULL);
        if (iResult == SOCKET_ERROR)
        {
            wprintf(L"WSAIoctl failed with error: %u\n", WSAGetLastError());
            closesocket(listen_socket_);
            return -1;
        }

        start_accept();

        return 0;
    }

    void destroy()
    {
        iocp_.destroy();
    }

    void update()
    {
        std::array<OVERLAPPED_ENTRY, 64> entries;
        size_t count = iocp_.poll(entries.data(), entries.size());
        for (size_t i = 0; i < count; ++i)
        {
            OVERLAPPED_ENTRY& entry = entries[i];
            async_context* ctx = (async_context*)entry.lpOverlapped;
            if (async_io_type::accept == ctx->type)
            {
                // ������һ���ͻ���������~
                accept_context* context = (accept_context*)ctx;
                std::unique_ptr<tcp_connection> connection = std::make_unique<tcp_connection>();
                iocp_.bind(context->handle, connection.get());
                connection->attach(context->handle);

                // �ź�֪ͨ
                sig_on_connection_(connection.get());

                // �ڸÿͻ����׽����Ϸ����첽recv����
                connection->start_recv();

                // ���浽�б���
                clients_.insert(std::make_pair(context->handle, std::move(connection)));
                
                // �ڼ����׽������ٷ���һ���첽accept����
                delete context;
                start_accept();
            }
            else if (async_io_type::recv == ctx->type)
            {
                recv_context* context = (recv_context*)ctx;
                context->connection->on_recv_data();
            }
        }
    }

private:
    int start_accept()
    {
        accept_context* context = new accept_context();
        if (!context) { return -1; }

        // �����׽���
        context->handle = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (context->handle == INVALID_SOCKET)
        {
            return -1;
        }

        // �����첽����
        DWORD dwBytesReceived = 0;
        BOOL bRetVal = lpfnAcceptEx(listen_socket_, context->handle, &context->local_addr, 0,
                                    sizeof(context->local_addr) + sizeof(context->local_padding),
                                    sizeof(context->remote_addr) + sizeof(context->remote_padding),
                                    &dwBytesReceived,
                                    &context->overlapped);
        if (bRetVal)
        {
            // �첽�������������
            //TODO:
            assert(false);
        }
        else
        {
            int error = WSAGetLastError();
            if (ERROR_IO_PENDING == error)
            {
                //TODO:
            }
            else
            {
                // �첽������ʧ����
                closesocket(context->handle);
                delete context;
                return -1;
            }
        }

        return 0;
    }

private:
    int const kMaxBackLog = 100;
    ULONG_PTR const kListenSockCompletionKey = 'vxer';

    iocp iocp_;
    SOCKET listen_socket_;
    LPFN_ACCEPTEX lpfnAcceptEx = NULL;
    std::unordered_map<SOCKET, std::unique_ptr<tcp_connection> > clients_;
};

class echo_server
{
public:
    int init()
    {
        server_.create(20023);
        server_.sig_on_connection_.connect(&echo_server::on_recv_connection, this);
        return 0;
    }

    void update()
    {
        server_.update();
    }

    void destroy()
    {
        server_.sig_on_connection_.disconnect_all();
    }

private:
    /** �յ�һ���ͻ������� */
    void on_recv_connection(tcp_connection* connection)
    {
        connection->sig_on_read_.connect(std::bind(&echo_server::on_recv_data, this, connection, std::placeholders::_1, std::placeholders::_2));
    }

    /** �յ����� */
    void on_recv_data(tcp_connection* connection, uint8_t* data, size_t sz)
    {
        char buffer[1024];
        assert(sz < _countof(buffer));
        memcpy(buffer, data, sz);
        buffer[sz] = 0;
        std::cout << buffer;
    }

private:
    tcp_server server_;
};

int main()
{
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != NO_ERROR)
    {
        std::cout <<L"Error at WSAStartup" << std::endl;
        return 1;
    }

    echo_server server;
    server.init();
    bool quit = false;
    while (!quit)
    {
        server.update();
    }
    server.destroy();

    WSACleanup();
    return 0;
}

/*
===============================================================================
1��OVERLAPPED���ܷ�ջ�ϣ����첽�������ǰ���ڴ����һֱ��Ч��
2��WSABUF[]����������鲻�������ڴ棬�ǳ��ʺϻ��λ�������β�пռ䡢�м�û�ռ�����
===============================================================================
*/
