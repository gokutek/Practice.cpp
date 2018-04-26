#include <assert.h>
#include <windows.h>
#include "i_lib_mt.h"

int main()
{
	HMODULE hModule = LoadLibraryA("libMT.dll");
	assert(hModule != NULL);

	typedef  ITest* (*pfn_t)();
	pfn_t fn = (pfn_t)GetProcAddress(hModule, "GetTest");
	assert(fn);

	ITest *pTest = fn();
	
	std::string const str = pTest->Crash();

	return 0;
}
