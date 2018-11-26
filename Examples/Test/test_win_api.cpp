#include <assert.h>
#include <iostream>
#include <Windows.h>
#include <Psapi.h>
#include <tlhelp32.h>
#include <string>
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


/*
============================
OpenMutex第一个参数不能指定0
============================
*/
TEST_CASE("Mutex", "WinAPI")
{
	HANDLE hMutex = CreateMutexW(NULL, FALSE, L"{C14C3FA8-C179-4E24-B435-784C7FF610BB}");
	REQUIRE(hMutex != NULL);

	HANDLE hMutex1 = OpenMutexW(0, FALSE, L"{C14C3FA8-C179-4E24-B435-784C7FF610BB}");
	REQUIRE(hMutex1 == NULL);
	CloseHandle(hMutex1);

	hMutex1 = OpenMutexW(MUTEX_ALL_ACCESS, FALSE, L"{C14C3FA8-C179-4E24-B435-784C7FF610BB}");
	REQUIRE(hMutex1 != NULL);
	CloseHandle(hMutex1);

	CloseHandle(hMutex);
}


TEST_CASE("EnumProcessModules", "WinAPI")
{
	DWORD cbNeeded = 0;
	EnumProcessModules(GetCurrentProcess(), NULL, 0, &cbNeeded);
	HMODULE *lphModule = (HMODULE*)malloc(cbNeeded);
	BOOL bRet = EnumProcessModules(GetCurrentProcess(), lphModule, cbNeeded, &cbNeeded);
	REQUIRE(bRet);
	for (size_t i = 0; i < cbNeeded / sizeof(HMODULE); ++i) {
		char file[256];
		GetModuleFileNameA(lphModule[i], file, sizeof(file));
	}
	free(lphModule);
}


TEST_CASE("CreateToolhelp32Snapshot", "WinAPI")
{
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, GetCurrentProcessId());
	REQUIRE(INVALID_HANDLE_VALUE != hSnapshot);
	MODULEENTRY32 me;
	memset(&me, 0, sizeof(me));
	me.dwSize = sizeof(me);
	BOOL bRet = Module32First(hSnapshot, &me);
	while (bRet) {
		bRet = Module32Next(hSnapshot, &me);
	}
	CloseHandle(hSnapshot);
}


/*
===============================================================================
使用template+auto，可以省去声明typedef函数签名，注意调用约定。
===============================================================================
*/
template<typename T>
T* GetProcAddressT(HMODULE hModule, char const* func)
{
	return (T*)GetProcAddress(hModule, func);
}


TEST_CASE("GetProcAddress", "WinAPI")
{
	auto pfnCreateFileW = GetProcAddressT<HANDLE __stdcall (LPCWSTR, DWORD, DWORD, LPSECURITY_ATTRIBUTES, DWORD, DWORD, HANDLE)>(GetModuleHandleW(L"kernel32"), "CreateFileW");
	REQUIRE(pfnCreateFileW != NULL);
	REQUIRE((void*)pfnCreateFileW == (void*)CreateFileW);
	HANDLE hd = pfnCreateFileW(L"C:\\62CC8542-C99F-4802-9950-2AC62E662686", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	REQUIRE(INVALID_HANDLE_VALUE == hd);
}


/*
===============================================================================
https://docs.microsoft.com/zh-cn/windows/desktop/api/shellapi/ns-shellapi-_shfileopstructa
This string must be double-null terminated.
===============================================================================
*/
static int RemoveFileToDustbin(char const *file)
{
	SHFILEOPSTRUCTA FileOp = { 0 };
	FileOp.fFlags = FOF_ALLOWUNDO | FOF_NOCONFIRMATION;  //允许放回回收站;
	FileOp.pFrom = file;
	FileOp.pTo = NULL;  //一定要是NULL
	FileOp.wFunc = FO_DELETE;//删除操作
	int const nRet = SHFileOperationA(&FileOp);
	return nRet;
}


TEST_CASE("RemoveFileToDustbin", "WinAPI")
{
	std::string file = "d:\\{46516991-A064-472A-BCEC-782B1FE0C704}.txt";
	FILE* fd = fopen(file.c_str(), "w");
	fclose(fd);
	file.push_back(0);
	int const nRet = RemoveFileToDustbin(file.c_str());
	REQUIRE(nRet == 0);
}


/**
 * 多字节转宽字符
 * @param str		字符串，可以是ANSI或者UTF-8
 * @param code_page	表示str的编码
 */
static std::wstring MultiByteToWideChar(char const *str, UINT code_page)
{
	std::wstring result;
	int const nSize = MultiByteToWideChar(code_page, 0, str, -1, NULL, 0);
	result.resize(nSize);
	int const nRet = MultiByteToWideChar(code_page, 0, str, -1, result.data(), nSize);
	assert(nRet == nSize);
	result.pop_back();
	return result;
}


/**
* 宽字符转多字节
* @param str		unicode_str
* @param code_page	要转换到的目标编码，可以是ANSI或者UTF-8
*/
static std::string WideCharToMultiByte(wchar_t const* unicode_str, UINT code_page)
{
	std::string result;
	int const nSize = WideCharToMultiByte(code_page, 0, unicode_str, -1, NULL, 0, NULL, NULL);
	result.resize(nSize);
	int const nRet = WideCharToMultiByte(code_page, 0, unicode_str, -1, result.data(), nSize, NULL, NULL);
	assert(nRet == nSize);
	result.pop_back();
	return result;
}


/*
===============
ANSI => UNICODE
===============
*/
static std::wstring AnsiToUnicode(char const *ansi_str)
{
	return MultiByteToWideChar(ansi_str, CP_ACP);
}



/*
===============
UNICODE => ANSI
===============
*/
static std::string UnicodeToAnsi(wchar_t const *unicode_str)
{
	return WideCharToMultiByte(unicode_str, CP_ACP);
}


/*
============
ANSI => UTF8
============
*/
static std::string AnsiToUtf8(char const *ansi_str)
{
	std::wstring const wstr = MultiByteToWideChar(ansi_str, CP_ACP);
	return WideCharToMultiByte(wstr.c_str(), CP_UTF8);
}


/*
============
UTF8 => ANSI
============
*/
static std::string Utf8ToAnsi(char const *utf8_str)
{
	std::wstring const wstr = MultiByteToWideChar(utf8_str, CP_UTF8);
	return WideCharToMultiByte(wstr.c_str(), CP_ACP);
}


TEST_CASE("Utf8ToAnsi", "WinAPI")
{
	std::wstring const unicode_str = AnsiToUnicode("hello中国");
	REQUIRE(unicode_str == L"hello中国");

	std::string const ansi_str = UnicodeToAnsi(unicode_str.c_str());
	REQUIRE(ansi_str == "hello中国");

	std::string const utf8_str = AnsiToUtf8("hello中国");
	std::string const ansi_str1 = Utf8ToAnsi(utf8_str.c_str());
	REQUIRE(ansi_str1 == "hello中国");
}
