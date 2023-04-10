/*
===============================================================================
2023/04/07: 移植MSDN上的demo
2023/04/10: 接口封装

参考: 
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
        //TODO:是不是对内存对齐有什么要求，放在std::vector里崩溃
        ULONG ulNumEntriesRemoved = 0;
        DWORD dwMillisecondsToWait = 0;
        GetQueuedCompletionStatusEx(iocp_handle_, entries, count, &ulNumEntriesRemoved, dwMillisecondsToWait, FALSE);
        return ulNumEntriesRemoved;
    }

private:
    HANDLE iocp_handle_ = NULL;
};

struct accept_context
{
    accept_context()
    {
        memset(this, 0, sizeof(*this));
    }

    SOCKET handle;
    WSAOVERLAPPED overlapped;
    uint8_t recv_data[512];
    sockaddr_in local_addr;
    uint8_t local_padding[16];
    sockaddr_in remote_addr;
    uint8_t remote_padding[16];
};

class tcp_connection
{
public:
    tcp_connection()
        : peer_socket_(INVALID_SOCKET)
    {
        recv_buffer_.reserve(1024);
    }

    int attach(SOCKET handle)
    {
        assert(peer_socket_ == INVALID_SOCKET);
        peer_socket_ = handle;
        return 0;
    }

    void close()
    {
    }

private:
    SOCKET peer_socket_;
    std::vector<uint8_t> recv_buffer_;
};

class tcp_server
{
public:
    int create(uint16_t port)
    {
        if (iocp_.create())
        {
            return -1;
        }

        // 创建监听套接字
        listen_socket_ = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (listen_socket_ == INVALID_SOCKET)
        {
            wprintf(L"Create of ListenSocket socket failed with error: %u\n", WSAGetLastError());
            return -1;
        }

        // 套接字与完成端口绑定
        iocp_.bind(listen_socket_, (void*)kListenSockCompletionKey);

        // 监听套接字绑定到本地IP+端口
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

        // 开始监听
        if (listen(listen_socket_, kMaxBackLog) == SOCKET_ERROR)
        {
            wprintf(L"listen failed with error: %u\n", WSAGetLastError());
            closesocket(listen_socket_);
            return -1;
        }

        // 获取MS扩展函数地址
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
            if (kListenSockCompletionKey == entry.lpCompletionKey)
            {
                // 接受了一个客户端新连接~
                accept_context* context = (accept_context*)entry.lpOverlapped;
                std::unique_ptr<tcp_connection> connection = std::make_unique<tcp_connection>();
                iocp_.bind(context->handle, connection.get());
                connection->attach(context->handle);
                std::cout << "hello listen" << std::endl;
                clients_.insert(std::make_pair(context->handle, std::move(connection)));

                // TODO:在该连接上发起recv请求
                // TODO:再发起一个异步accept
            }
            else
            {
                assert(false);
            }
        }
    }

private:
    int start_accept()
    {
        accept_context* context = new accept_context();
        if (!context) { return -1; }

        // 分配套接字
        context->handle = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (context->handle == INVALID_SOCKET)
        {
            return -1;
        }

        // 发起异步请求
        DWORD dwBytesReceived = 0;
        BOOL bRetVal = lpfnAcceptEx(listen_socket_, context->handle, context->recv_data,
                                    sizeof(context->recv_data),
                                    sizeof(context->local_addr) + sizeof(context->local_padding),
                                    sizeof(context->remote_addr) + sizeof(context->remote_padding),
                                    &dwBytesReceived,
                                    &context->overlapped);
        if (bRetVal)
        {
            // 异步请求立即完成了
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
                // 异步请求发起失败了
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
};

int main()
{
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != NO_ERROR)
    {
        std::cout <<L"Error at WSAStartup" << std::endl;
        return 1;
    }

    tcp_server server;
    server.create(20023);
    bool quit = false;
    while (!quit)
    {
        server.update();
    }
    server.destroy();

    WSACleanup();
    return 0;

#if 0
    // Associate the accept socket with the completion port
    HANDLE hCompPort2 = CreateIoCompletionPort((HANDLE)AcceptSocket, hCompPort, (u_long)0, 0);
    // hCompPort2 should be hCompPort if this succeeds
    if (hCompPort2 == NULL) 
    {
        wprintf(L"CreateIoCompletionPort associate failed with error: %u\n", GetLastError());
        closesocket(AcceptSocket);
        closesocket(ListenSocket);
        return 1;
    }

    // Continue on to use send, recv, TransmitFile(), etc.,.
    //...
#endif
}

/*
===============================================================================
1，OVERLAPPED不能放栈上，在异步请求完成前，内存必须一直有效；
===============================================================================
*/
