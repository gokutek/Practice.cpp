/*
===============================================================================
 * 创建时间: 2023/12/01
===============================================================================
 */

#include <WinSock2.h>
#include <Windows.h>
#include <memory.h>
#include <stdint.h>
#include <iostream>
#include <memory>
#include <queue>
#include <string>
#include <unordered_map>
#include <thread>
#include <mutex>
#include "kcp/ikcp.h"

#pragma comment(lib, "ws2_32.lib")

#define PORT        8080 
#define MAXLINE     1024 

/**
 * @brief 为了支持sockaddr_in作为unordered_map的Key
 * 
 * https://stackoverflow.com/questions/17016175/c-unordered-map-using-a-custom-class-type-as-the-key
 */
template <>
struct std::hash<sockaddr_in>
{
    std::size_t operator()(sockaddr_in const& addr) const
    {
        return std::hash<int>()(addr.sin_family)
            ^ std::hash<int>()(addr.sin_port)
            ^ std::hash<int>()(addr.sin_addr.S_un.S_addr);
    }
};

/**
 * @brief 为了支持sockaddr_in作为unordered_map的Key
 * @param A 
 * @param B 
 * @return 
 */
bool operator==(sockaddr_in const& A, sockaddr_in const& B)
{
    return (memcmp(&A, &B, sizeof(A)) == 0);
}

/**
 * @brief 表示一条连接信息，同时作为ikcp_create时绑定的user data。
 */
struct connection_info_t
{
    SOCKET sockfd = INVALID_SOCKET;
    sockaddr_in addr;
    ikcpcb* kcp = nullptr;
};

/**
 * @brief 提供给kcp的回调接口，将数据写入socket
 * @param buf 
 * @param len 
 * @param kcp 
 * @param user 
 * @return 
 */
static int udp_output(const char* buf, int len, ikcpcb* kcp, void* user)
{
    connection_info_t* connection_info = (connection_info_t*)user;
    return sendto(connection_info->sockfd, buf, len, 0, (sockaddr*)&connection_info->addr, len);
}

/**
 * @brief 连接管理。实现sockaddr_in到connection_info_t的映射
 */
class connection_manager
{
public:
    int add(sockaddr_in const& addr)
    {
        if (connections_.count(addr) > 0)
        {
            return -1; //已存在
        }
        connections_.insert(std::make_pair(addr, std::make_unique<connection_info_t>()));
        return 0;
    }

    int remove(sockaddr_in const& addr)
    {
        if (connections_.erase(addr) > 0)
        {
            return 0;
        }
        return -1; //不存在
    }

    connection_info_t* get_connection_info(sockaddr_in const& addr)
    {
        auto iter = connections_.find(addr);
        if (iter == connections_.end()) { return nullptr; }
        return iter->second.get();
    }

    void update(IUINT32 current)
    {
        for (auto& iter : connections_)
        {
            if (iter.second->kcp)
            {
                ikcp_update(iter.second->kcp, current);
            }
        }
    }

private:
    std::unordered_map<sockaddr_in, std::unique_ptr<connection_info_t> > connections_;
};

/**
 * @brief 设置socket的阻塞/非阻塞模式
 * @param sock 
 * @param isblock 
 * @return 
 */
static bool set_socket_block(SOCKET sock, bool isblock)
{
    unsigned long ul = isblock ? 0 : 1;
    return ioctlsocket(sock, FIONBIO, (unsigned long*)&ul);
}

/**
 * @brief 服务器
 */
class udp_server
{
public:
    int udp_server_main()
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

        set_socket_block(sockfd, false);

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
            connection_manager_.update(clock());

            //-----------------------------------------------
            // Call the recvfrom function to receive datagrams
            // on the bound socket.
            sockaddr_in cliaddr;
            memset(&cliaddr, 0, sizeof(cliaddr));
            int len = sizeof(cliaddr);  //len is value/result 

            char buffer[MAXLINE] = { 0 };
            int n = recvfrom(sockfd, buffer, sizeof(buffer), 0, (sockaddr*)&cliaddr, &len);
            if (n <= 0) { continue; }

            connection_manager_.add(cliaddr);
            connection_info_t* connection_info = connection_manager_.get_connection_info(cliaddr);
            if (!connection_info->kcp)
            {
                connection_info->kcp = ikcp_create(1, connection_info); //FIXME:第一个参数
                assert(connection_info->kcp);
                connection_info->kcp->output = udp_output;

                connection_info->addr = cliaddr;
                connection_info->sockfd = sockfd;
            }

            ikcp_input(connection_info->kcp, buffer, n);

            memset(buffer, 0, sizeof(buffer));
            n = ikcp_recv(connection_info->kcp, buffer, MAXLINE);
            if (n > 0)
            {
                std::cout << "Recv from client: " << buffer << std::endl;

                // Echo
                _strupr(buffer);
                ikcp_send(connection_info->kcp, buffer, n);
            }
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

private:
    connection_manager connection_manager_;
};

/**
 * @brief 客户端
 */
class udp_client
{
public:
    int udp_client_main()
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

        set_socket_block(sockfd, false);

        //-----------------------------------------------
        // Bind the socket to any address and the specified port.
        sockaddr_in servaddr;
        memset(&servaddr, 0, sizeof(servaddr));
        servaddr.sin_family = AF_INET; // IPv4 
        servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
        servaddr.sin_port = htons(PORT);

        connection_manager_.add(servaddr);
        connection_info_t* connection_info = connection_manager_.get_connection_info(servaddr);
        connection_info->kcp = ikcp_create(1, connection_info); //FIXME:第一个参数
        assert(connection_info->kcp);
        connection_info->kcp->output = udp_output;
        connection_info->addr = servaddr;
        connection_info->sockfd = sockfd;

        int index = 0;
        while (true)
        {
            connection_manager_.update(clock());

            char buffer[MAXLINE] = { 0 };
            sprintf(buffer, "msg %d", index++);

            ikcp_send(connection_info->kcp, buffer, strlen(buffer));

            //-----------------------------------------------
            // Call the recvfrom function to receive datagrams
            // on the bound socket.
            sockaddr_in cliaddr;
            memset(&cliaddr, 0, sizeof(cliaddr));
            int len = sizeof(cliaddr);  //len is value/result 

            int n = recvfrom(sockfd, buffer, sizeof(buffer), 0, (sockaddr*)&cliaddr, &len);
            if (n > 0)
            {
                ikcp_input(connection_info->kcp, buffer, n);
            }

            memset(buffer, 0, sizeof(buffer));
            n = ikcp_recv(connection_info->kcp, buffer, MAXLINE);
            if (n > 0)
            {
                std::cout << "Recv from server: " << buffer << std::endl;
            }

            Sleep(1000);
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

private:
    connection_manager connection_manager_;
};

int main(int argc, char** argv)
{
    if (argc != 2)
    {
        return 0;
    }

    if (!strcmp(argv[1], "/s"))
    {
        udp_server server;
        return server.udp_server_main();
    }
    else if (!strcmp(argv[1], "/c"))
    {
        udp_client client;
        return client.udp_client_main();
    }

    return 0;
}

/*
-------------------------------------------------------------------------------
参考资料：
1，https://github.com/tidus5/kcp_demo
2，Practice.cpp/source/test/test_udp_socket.cpp

kcp简介：
1，kcp底层不关心是用tcp还是udp发数据；
2，类似算法中的“空间换时间”，kcp用“流量换时间”，以一定的流程消耗来确保低延迟；

将tcp/udp改为使用kcp的核心要点：
1，在建立C/S的连接后，创建相应的kcp；
2，socket收到的数据塞给kcp后，再从kcp拿数据；

为了简单化，本DEMO还是单线程的。
-------------------------------------------------------------------------------
*/
