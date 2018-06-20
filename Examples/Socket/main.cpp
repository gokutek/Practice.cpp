#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>
#include <windows.h>


int main()
{
    WSADATA data;
    WSAStartup(MAKEWORD(2, 2), &data);
    SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
    addr.sin_port = htons(55566); // 不能直接写55566
    int ret = bind(sock, (sockaddr*)&addr, sizeof(addr));
    ret = listen(sock, 100);
    WSACleanup();
    return 0;
}
