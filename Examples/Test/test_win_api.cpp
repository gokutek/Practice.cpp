#include <Windows.h>
#include "catch.hpp"

/*
========================
Windows系统API的一些测试
========================
*/

/*
===============================================================================
创建进程时，可以让新进程处于suspended状态，之后使用ResumeThread让新进程开始运行
===============================================================================
*/
TEST_CASE("CreateProcess", "WinAPI")
{
	wchar_t args[512] = L"notepad.exe";
	STARTUPINFO si;
	memset(&si, 0, sizeof(si));
	si.cb = sizeof(si);
	PROCESS_INFORMATION pi;
	memset(&pi, 0, sizeof(pi));
	BOOL bRet = CreateProcessW(NULL, args, NULL, NULL, FALSE, CREATE_SUSPENDED, NULL, NULL, &si, &pi);
	REQUIRE(bRet != FALSE);

	ResumeThread(pi.hThread);

	TerminateProcess(pi.hProcess, 0);

	CloseHandle(pi.hThread);
	CloseHandle(pi.hProcess);
}
