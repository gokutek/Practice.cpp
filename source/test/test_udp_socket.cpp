/*
 * 创建时间: 2023/11/15
 * 参考：
 *      https://www.geeksforgeeks.org/udp-server-client-implementation-c/
 *      https://learn.microsoft.com/en-us/windows/win32/api/winsock/nf-winsock-sendto
 */

#include <WinSock2.h>
#include <Windows.h>
#include <memory.h>
#include <iostream>
#include <WS2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

#define PORT        8080 
#define MAXLINE     1024 

static int udp_server_main()
{
    //-----------------------------------------------
    // Initialize Winsock
    WSADATA wsaData;
    int  iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != NO_ERROR) 
    {
        std::cout << "WSAStartup failed with error: " << iResult << std::endl;
        return 1;
    }

    //-----------------------------------------------
    // Create a receiver socket to receive datagrams
    SOCKET sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (INVALID_SOCKET == sockfd)
    {
        std::cout << "socket creation failed" << std::endl;
        exit(EXIT_FAILURE);
    }

    //-----------------------------------------------
    // Bind the socket to any address and the specified port.
    sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET; // IPv4 
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);

    if (0 != bind(sockfd, (sockaddr*)&servaddr, sizeof(servaddr)))
    {
        std::cout << "bind failed." << std::endl;
        exit(EXIT_FAILURE);
    }

    while (true)
    {
        //-----------------------------------------------
        // Call the recvfrom function to receive datagrams
        // on the bound socket.
        sockaddr_in cliaddr;
        memset(&cliaddr, 0, sizeof(cliaddr));
        int len = sizeof(cliaddr);  //len is value/result 

        char buffer[MAXLINE] = { 0 };
        int n = recvfrom(sockfd, buffer, sizeof(buffer), 0, (sockaddr*)&cliaddr, &len);
        std::cout << "Recv from client: " << buffer << std::endl;

        // Echo
        _strupr(buffer);
        sendto(sockfd, buffer, n, 0, (sockaddr*)&cliaddr, len);
    }

    //-----------------------------------------------
    // Close the socket when finished receiving datagrams
    std::cout << "Finished receiving. Closing socket." << std::endl;
    iResult = closesocket(sockfd);
    if (iResult == SOCKET_ERROR) 
    {
        std::cout << "closesocket failed with error " << WSAGetLastError() << std::endl;
        return 1;
    }

    //-----------------------------------------------
    // Clean up and exit.
    WSACleanup();

    std::cout << "Exiting." << std::endl;

    return 0;
}

static int udp_client_main()
{
    //-----------------------------------------------
    // Initialize Winsock
    WSADATA wsaData;
    int  iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != NO_ERROR)
    {
        std::cout << "WSAStartup failed with error: " << iResult << std::endl;
        return 1;
    }

    //-----------------------------------------------
    // Create a receiver socket to receive datagrams
    SOCKET sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (INVALID_SOCKET == sockfd)
    {
        std::cout << "socket creation failed" << std::endl;
        exit(EXIT_FAILURE);
    }

    //-----------------------------------------------
    // Bind the socket to any address and the specified port.
    sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET; // IPv4 
    inet_pton(AF_INET, "127.0.0.1", &servaddr.sin_addr);
    servaddr.sin_port = htons(PORT);

    while (true)
    {
        std::string str;
        std::cout << "Input message: ";
        std::cin >> str;

        sendto(sockfd, str.c_str(), str.size(), 0, (sockaddr*)&servaddr, sizeof(servaddr));

        //-----------------------------------------------
        // Call the recvfrom function to receive datagrams
        // on the bound socket.
        sockaddr_in cliaddr;
        memset(&cliaddr, 0, sizeof(cliaddr));
        int len = sizeof(cliaddr);  //len is value/result 

        char buffer[MAXLINE] = { 0 };
        int n = recvfrom(sockfd, buffer, sizeof(buffer), 0, (sockaddr*)&cliaddr, &len);
        std::cout << "Recv from server: " << buffer << std::endl;
    }

    //-----------------------------------------------
    // Close the socket when finished receiving datagrams
    std::cout << "Finished receiving. Closing socket." << std::endl;
    iResult = closesocket(sockfd);
    if (iResult == SOCKET_ERROR)
    {
        std::cout << "closesocket failed with error " << WSAGetLastError() << std::endl;
        return 1;
    }

    //-----------------------------------------------
    // Clean up and exit.
    WSACleanup();

    std::cout << "Exiting." << std::endl;

    return 0;
}

int main(int argc, char** argv)
{
	if (argc != 2)
	{
		return 0;
	}

    if (!strcmp(argv[1], "/s"))
    {
        return udp_server_main();
    }
    else if (!strcmp(argv[1], "/c"))
    {
        return udp_client_main();
    }

	return 0;
}
