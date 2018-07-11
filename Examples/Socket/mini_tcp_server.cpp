#include <assert.h>
#include <set>
#include "mini_tcp_server.h"

#ifdef _MSC_VER
#include <WinSock2.h>
#endif // _MSC_VER


class EasyTcpServer : public IMiniTcpServer
{
public:
    EasyTcpServer();

    int Listen(char const *ip, int port) override;

    int Close() override;

    void SetOnConnectionCallback(on_connection_cb_t const cb, void *ud) override;

    void SetOnReadCallback(on_read_cb_t const &cb, void *ud) override;

    void SetOnCloseCallback(on_close_cb_t const &cb, void *ud) override;

    int Send(peer_t peer, void const *data, size_t sz) override;

    int Disconnect(peer_t peer) override;

    int Poll();

private:
    on_connection_cb_t  on_conn_cb_;
    void                *on_conn_cb_ud_;
    on_read_cb_t        on_read_cb_;
    void                *on_read_cb_ud_;
    on_close_cb_t       on_close_cb_;
    void                *on_close_cb_ud_;

    SOCKET              listenSock_;
    std::set<SOCKET>    clientSocks_;
};


// 将套接字设为非阻塞模式
static int SetNonBlock(SOCKET sock)
{
    u_long argp = 1;
    int const ret = ioctlsocket(sock, FIONBIO, &argp);
    return ret;
}


EasyTcpServer::EasyTcpServer()
{
    on_conn_cb_ = NULL;
    on_conn_cb_ud_ = NULL;
    on_read_cb_ = NULL;
    on_read_cb_ud_ = NULL;
    on_close_cb_ = NULL;
    on_close_cb_ud_ = NULL;
    listenSock_ = INVALID_SOCKET;
}


int EasyTcpServer::Listen(char const *ip, int port)
{
    listenSock_ = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    assert(listenSock_ != INVALID_SOCKET);
    int ret = SetNonBlock(listenSock_);
    assert(0 == ret);

    sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.S_un.S_addr = inet_addr(ip);
    addr.sin_port = htons(port);
    ret = bind(listenSock_, (sockaddr*)&addr, sizeof(addr));
    assert(0 == ret);

    ret = listen(listenSock_, 100);
    assert(0 == ret);

    return -1;
}


int EasyTcpServer::Close()
{
    closesocket(listenSock_);
    listenSock_ = INVALID_SOCKET;

    for (std::set<SOCKET>::iterator iter = clientSocks_.begin(); iter != clientSocks_.end(); ++iter) {
        closesocket(*iter);
    }
    clientSocks_.clear();

    return 0;
}


void EasyTcpServer::SetOnConnectionCallback(on_connection_cb_t const cb, void *ud)
{
    on_conn_cb_ = cb;
    on_conn_cb_ud_ = ud;
}


void EasyTcpServer::SetOnReadCallback(on_read_cb_t const &cb, void *ud)
{
    on_read_cb_ = cb;
    on_read_cb_ud_ = ud;
}


void EasyTcpServer::SetOnCloseCallback(on_close_cb_t const &cb, void *ud)
{
    on_close_cb_ = cb;
    on_close_cb_ud_ = ud;
}


int EasyTcpServer::Send(peer_t peer, void const *data, size_t sz)
{
    SOCKET sock = (SOCKET)peer;
    int ret = send(sock, (const char*)data, (int)sz, 0);
    return ret;
}


int EasyTcpServer::Disconnect(peer_t peer)
{
    SOCKET sock = (SOCKET)peer;

    std::set<SOCKET>::iterator iter = clientSocks_.find(sock);
    if (iter == clientSocks_.end()) {
        return -1;
    }

    closesocket(sock);
    clientSocks_.erase(iter);
    return 0;
}


int EasyTcpServer::Poll()
{
    fd_set readfds;
    FD_ZERO(&readfds);
    fd_set writefds;
    FD_ZERO(&writefds);
    fd_set exceptfds;
    FD_ZERO(&exceptfds);

    FD_SET(listenSock_, &readfds);
    for (std::set<SOCKET>::const_iterator iter = clientSocks_.begin(); iter != clientSocks_.end(); ++iter) {
        FD_SET(*iter, &readfds);
    }
    
    timeval timeout;
    memset(&timeout, 0, sizeof(timeout));
    int ret = select(0, &readfds, &writefds, &exceptfds, &timeout);
    
    if (0 == ret) { 
        return 0; 
    }

    if (SOCKET_ERROR == ret) {
        return -1;
    }

    for (size_t i = 0; i < readfds.fd_count; ++i) {
        SOCKET sock = readfds.fd_array[i];
        if (sock != listenSock_) {
            static char buf[4096];
            ret = recv(sock, buf, sizeof(buf), 0);
            if (SOCKET_ERROR == ret || 0 == ret) {
                closesocket(sock);
                std::set<SOCKET>::iterator iter = clientSocks_.find(sock);
                if (iter != clientSocks_.end()) {
                    clientSocks_.erase(sock);
                    on_close_cb_(on_close_cb_ud_, sock);
                }
            } else {
                on_read_cb_(on_read_cb_ud_, sock, buf, ret);
            }
        } else {
            SOCKET clientSock = accept(listenSock_, NULL, NULL);

            if (INVALID_SOCKET == clientSock) {
                continue;
            }

            clientSocks_.insert(clientSock);

            ret = SetNonBlock(clientSock);
            assert(0 == ret);

            on_conn_cb_(on_conn_cb_ud_, clientSock);
        }
    }

    return 0;
}


IMiniTcpServer* CreateServer()
{
    return new EasyTcpServer();
}
