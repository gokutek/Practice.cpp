#include <windows.h>
#include "i_lib_mt.h"

bool WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	switch (fdwReason)
	{
	case DLL_PROCESS_ATTACH:
		break;
	}
	return true;
}


class Test : public ITest
{
public:
	std::string Crash() override
	{
		std::string const str = "Hello";
		return str;
	}
};


extern "C" __declspec(dllexport) ITest* GetTest()
{
	static Test obj;
	return &obj;
}
