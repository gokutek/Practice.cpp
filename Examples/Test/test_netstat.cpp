#ifdef _MSC_VER

#include <assert.h>
#include <Winsock2.h>
#include <Windows.h>
#include <Psapi.h>
#include <Iprtrmib.h>
#include <vector>
#include <algorithm>
#include <iostream>
#include "catch.hpp"

#pragma comment(lib, "Psapi.lib")
#pragma comment(lib, "Iphlpapi.Lib")
#pragma comment(lib, "WS2_32.lib")

/*
===============================================================================
获取本机上所有的侦听的端口号，使用的私有API
===============================================================================
*/


struct MIB_TCPEXROW_VISTA
{
	DWORD dwState;      // 连接状态
	DWORD dwLocalAddr;  // 本地地址
	DWORD dwLocalPort;  // 本地端口
	DWORD dwRemoteAddr; // 远程地址
	DWORD dwRemotePort; // 远程端口
	DWORD dwProcessId;  // 进程标识
	DWORD Unknown;      // 待定标识
};


struct MIB_TCPEXTABLE_VISTA
{
	DWORD dwNumEntries;
	MIB_TCPEXROW_VISTA table[ANY_SIZE];
};


typedef DWORD(WINAPI *PFNInternalGetTcpTable2)(MIB_TCPEXTABLE_VISTA **ppTcpTable, HANDLE heap, DWORD flags);


struct Attach
{
	std::string	name;	// 进程名称
	DWORD		pid;	// 进程ID
	int			port;	// 端口号（本机字节序）
};


static int netstat(std::vector<Attach> &list)
{
	HMODULE hModule = LoadLibraryW(L"iphlpapi.dll");
	if (hModule == NULL) { 
		return -1;
	}

	PFNInternalGetTcpTable2 pInternalGetTcpTable2 = (PFNInternalGetTcpTable2)GetProcAddress(hModule, "InternalGetTcpTable2");
	if (NULL == pInternalGetTcpTable2) {
		return -2;
	}

	MIB_TCPEXTABLE_VISTA *pTcpExTable = NULL;

	DWORD dwRet = pInternalGetTcpTable2(&pTcpExTable, GetProcessHeap(), 1);
	if (dwRet) {
		if (pTcpExTable) {
			HeapFree(GetProcessHeap(), 0, pTcpExTable);
		}
		return -3;
	}

	for (UINT i = 0; i < pTcpExTable->dwNumEntries; ++i) {
		if (pTcpExTable->table[i].dwState != 2) {
			continue;
		}

		HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pTcpExTable->table[i].dwProcessId);
		char path[1024] = { 0 };
		GetModuleFileNameExA(hProcess, NULL, path, sizeof(path));

		Attach item;
		item.name = path;
		item.pid = pTcpExTable->table[i].dwProcessId;
		item.port = ntohs(0x0000FFFF & pTcpExTable->table[i].dwLocalPort);
		list.emplace_back(item);
	}

	if (pTcpExTable) {
		HeapFree(GetProcessHeap(), 0, pTcpExTable);
	}

	return 0;
}


TEST_CASE("netstat", "netstat")
{
	DWORD const dwThisPID = GetCurrentProcessId();

	for (int test_count = 0, i = 0; test_count < 100; ++i) {
		SOCKET listenSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

		sockaddr_in addr;
		memset(&addr, 0, sizeof(addr));
		addr.sin_family = AF_INET;
		addr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
		int const bindPort = 30000 + i;
		addr.sin_port = htons(bindPort);
		int ret = bind(listenSock, (sockaddr*)&addr, sizeof(addr));
		if (ret) {
			closesocket(listenSock);
			continue;
		}

		ret = listen(listenSock, 100);
		if (ret) {
			closesocket(listenSock);
			continue;
		}

		std::vector<Attach> list;
		REQUIRE(0 == netstat(list));

		bool found = false;
		for (auto &item : list) {
			if (item.pid == dwThisPID && item.port == bindPort) {
				found = true;
				break;
			}
		}

		REQUIRE(found);

		closesocket(listenSock);
		++test_count;
	}
}

#endif // _MSC_VER
