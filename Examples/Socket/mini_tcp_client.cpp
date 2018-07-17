#include <assert.h>
#include "mini_tcp_client.h"


MiniTcpClient::MiniTcpClient()
    : sock_(INVALID_SOCKET)
{
    on_connect_ = NULL;
    on_connect_ud_ = NULL;
    on_read_ = NULL;
    on_read_ud_ = NULL;
    on_close_ = NULL;
    on_close_ud_ = NULL;
}


MiniTcpClient::~MiniTcpClient()
{
}


int MiniTcpClient::Connect(char const *ip, int port)
{
    connected_ = false;
    sock_ = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock_ == INVALID_SOCKET)
    {
        assert(0);
        return -1;
    }

    u_long argp = 1;
    int ret = ioctlsocket(sock_, FIONBIO, &argp);
    assert(!ret);

    sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.S_un.S_addr = inet_addr(ip);
    server_addr.sin_port = htons(port);

    ret = connect(sock_, (sockaddr*)&server_addr, sizeof(server_addr));
    if (!ret) 
    {
        connected_ = true;
        on_connect_(on_connect_ud_);
        return 0;
    }

    int err = WSAGetLastError();
    if (WSAEWOULDBLOCK == err) 
    { 
        return 0; 
    }

    return -1;
}


int MiniTcpClient::Close()
{
    return 0;
}


int MiniTcpClient::Send(void const *data, size_t sz)
{
    if (INVALID_SOCKET == sock_)
    {
        return -1;
    }

    int ret = send(sock_, (char const*)data, sz, 0);
    assert(SOCKET_ERROR != ret);
    assert(ret == sz);
    return 0;
}


void MiniTcpClient::SetOnConnectCallback(on_connect_cb_t cb, void *ud)
{
    on_connect_ = cb;
    on_connect_ud_ = ud;
}


void MiniTcpClient::SetOnReadCallback(on_read_cb_t cb, void *ud)
{
    on_read_ = cb;
    on_read_ud_ = ud;
}


void MiniTcpClient::SetOnCloseCallback(on_close_cb_t cb, void *ud)
{
    on_close_ = cb;
    on_close_ud_ = ud;
}


int MiniTcpClient::Poll()
{
    if (INVALID_SOCKET == sock_)
    {
        return -1;
    }

    fd_set readfds;
    FD_ZERO(&readfds);
    fd_set writefds;
    FD_ZERO(&writefds);
    fd_set exceptfds;
    FD_ZERO(&exceptfds);

    FD_SET(sock_, &readfds);
    
    if (!connected_) 
    {
        FD_SET(sock_, &writefds);
        FD_SET(sock_, &exceptfds);
    }

    timeval timeout;
    memset(&timeout, 0, sizeof(timeout));
    int ret = select(0, &readfds, &writefds, &exceptfds, &timeout);

    if (!ret)
    {
        return 0;
    }

    if (SOCKET_ERROR == ret)
    {
        assert(0);
        return -1;
    }

    if (exceptfds.fd_count > 0)
    {
        // 与服务器连接失败
        closesocket(sock_);
        sock_ = INVALID_SOCKET;
        on_close_(on_close_ud_);
        return 0;
    }

    if (writefds.fd_count > 0)
    {
        // 与服务器连接成功
        connected_ = true;
        on_connect_(on_connect_ud_);
        return 0;
    }

    if (readfds.fd_count > 0)
    {
        static char buf[4096];
        ret = recv(sock_, buf, sizeof(buf), 0);
        if (SOCKET_ERROR == ret || 0 == ret)
        {
            closesocket(sock_);
            sock_ = INVALID_SOCKET;
            on_close_(on_close_ud_);
        }
        else
        {
            on_read_(on_read_ud_, buf, ret);
        }
    }
    return 0;
}
