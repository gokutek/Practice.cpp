#include <WinSock2.h>
#include <windows.h>
#include <iostream>
#define CATCH_CONFIG_RUNNER
#include "catch.hpp"


int main(int argc, char* argv[])
{
    char* S = "";
    if (S[0] == '\0')
    {
        std::cout << "1" << std::endl;
    }
    else
    {
        std::cout << "0" << std::endl;
    }
	return 0;
}
