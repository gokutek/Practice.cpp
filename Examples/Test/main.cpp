#include <WinSock2.h>
#include <windows.h>
#include <iostream>
#define CATCH_CONFIG_RUNNER
#include "catch.hpp"


int main(int argc, char* argv[])
{
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

    int const res = Catch::Session().run(argc, argv);

	WSACleanup();
	
	return res;
}
