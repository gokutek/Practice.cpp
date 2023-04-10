/*
===============================================================================
https://learn.microsoft.com/en-us/windows/win32/api/mswsock/nf-mswsock-acceptex
2023/04/07: 移植demo
===============================================================================
*/

#ifndef UNICODE
#define UNICODE
#endif

#define WIN32_LEAN_AND_MEAN

#include <winsock2.h>
#include <ws2tcpip.h>
#include <mswsock.h>
#include <stdio.h>
#include <assert.h>

// Need to link with Ws2_32.lib
#pragma comment(lib, "Ws2_32.lib")

class iocp
{
public:
    void create()
    {
        assert(NULL == iocp_handle_);
        iocp_handle_ = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, (u_long)0, 0);
    }

    void destroy()
    {
        CloseHandle(iocp_handle_);
    }

    void bind(SOCKET socket, void* ud)
    {
        assert(NULL != iocp_handle_);
        HANDLE result = CreateIoCompletionPort((HANDLE)socket, iocp_handle_, (ULONG_PTR)ud, 0);
        assert(result == iocp_handle_);
    }

private:
    HANDLE iocp_handle_ = NULL;
};

int main()
{
    // 初始化Winsock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != NO_ERROR)
    {
        wprintf(L"Error at WSAStartup\n");
        return 1;
    }

    //----------------------------------------
    // Declare and initialize variables
    int iResult = 0;
    BOOL bRetVal = FALSE;

    char lpOutputBuf[1024];
    int outBufLen = 1024;
    DWORD dwBytes;

    // 创建完成端口
    HANDLE hCompPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, (u_long)0, 0);
    if (hCompPort == NULL) {
        wprintf(L"CreateIoCompletionPort failed with error: %u\n", GetLastError());
        WSACleanup();
        return 1;
    }

    // 创建监听套接字
    SOCKET ListenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (ListenSocket == INVALID_SOCKET) 
    {
        wprintf(L"Create of ListenSocket socket failed with error: %u\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    // 套接字与完成端口绑定
    CreateIoCompletionPort((HANDLE)ListenSocket, hCompPort, (u_long)0, 0);

    // 监听套接字绑定到本地IP+端口
    u_short port = 27015;
    hostent* thisHost = gethostbyname("");
    char* ip = inet_ntoa(*(struct in_addr*)*thisHost->h_addr_list);
    sockaddr_in service;
    service.sin_family = AF_INET;
    service.sin_addr.s_addr = inet_addr(ip);
    service.sin_port = htons(port);
    if (bind(ListenSocket, (SOCKADDR*)&service, sizeof(service)) == SOCKET_ERROR)
    {
        wprintf(L"bind failed with error: %u\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }

    // 开始监听
    iResult = listen(ListenSocket, 100);
    if (iResult == SOCKET_ERROR) 
    {
        wprintf(L"listen failed with error: %u\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }

    wprintf(L"Listening on address: %s:%d\n", ip, port);

    // Load the AcceptEx function into memory using WSAIoctl.
    // The WSAIoctl function is an extension of the ioctlsocket()
    // function that can use overlapped I/O. The function's 3rd
    // through 6th parameters are input and output buffers where
    // we pass the pointer to our AcceptEx function. This is used
    // so that we can call the AcceptEx function directly, rather
    // than refer to the Mswsock.lib library.
    LPFN_ACCEPTEX lpfnAcceptEx = NULL;
    GUID GuidAcceptEx = WSAID_ACCEPTEX;
    iResult = WSAIoctl(ListenSocket, SIO_GET_EXTENSION_FUNCTION_POINTER,
                       &GuidAcceptEx, sizeof(GuidAcceptEx),
                       &lpfnAcceptEx, sizeof(lpfnAcceptEx),
                       &dwBytes, NULL, NULL);
    if (iResult == SOCKET_ERROR) 
    {
        wprintf(L"WSAIoctl failed with error: %u\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }

    // Create an accepting socket
    SOCKET AcceptSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (AcceptSocket == INVALID_SOCKET) 
    {
        wprintf(L"Create accept socket failed with error: %u\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }

    // Empty our overlapped structure and accept connections.
    WSAOVERLAPPED olOverlap;
    memset(&olOverlap, 0, sizeof(olOverlap));

    bRetVal = lpfnAcceptEx(ListenSocket, AcceptSocket, lpOutputBuf,
        outBufLen - ((sizeof(sockaddr_in) + 16) * 2),
        sizeof(sockaddr_in) + 16, sizeof(sockaddr_in) + 16,
        &dwBytes, &olOverlap);
    if (bRetVal == FALSE) 
    {
        //ERROR_IO_PENDING这个不应该当做错误来处理
        wprintf(L"AcceptEx failed with error: %u\n", WSAGetLastError());
        closesocket(AcceptSocket);
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }

    // Associate the accept socket with the completion port
    HANDLE hCompPort2 = CreateIoCompletionPort((HANDLE)AcceptSocket, hCompPort, (u_long)0, 0);
    // hCompPort2 should be hCompPort if this succeeds
    if (hCompPort2 == NULL) 
    {
        wprintf(L"CreateIoCompletionPort associate failed with error: %u\n", GetLastError());
        closesocket(AcceptSocket);
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }

    // Continue on to use send, recv, TransmitFile(), etc.,.
    //...

    return 0;
}
