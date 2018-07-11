#include <iostream>
#include <windows.h>
#include "mini_tcp_server.h"


static void test_htons()
{
    SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
    addr.sin_port = htons(55566); // 不能直接写55566
    int ret = bind(sock, (sockaddr*)&addr, sizeof(addr));
    ret = listen(sock, 100);
    closesocket(sock);
}


static void OnConnection(void *ud, peer_t peer)
{
    std::cout << __FUNCTION__ << ": " << peer << std::endl;
}


static void OnRead(void *ud, peer_t peer, void *data, size_t sz)
{
    IMiniTcpServer *server = (IMiniTcpServer*)ud;
    server->Send(peer, data, sz);
}


static void OnClose(void *ud, peer_t peer)
{
    std::cout << __FUNCTION__ << ": " << peer << std::endl;
}


int main()
{
    WSADATA data;
    WSAStartup(MAKEWORD(2, 2), &data);

    IMiniTcpServer *server = CreateServer();
    int ret = server->Listen("127.0.0.1", 8088);
    server->SetOnConnectionCallback(OnConnection, NULL);
    server->SetOnReadCallback(OnRead, server);
    server->SetOnCloseCallback(OnClose, NULL);

    while (true) {
        server->Poll();
    }

    WSACleanup();
    return 0;
}
