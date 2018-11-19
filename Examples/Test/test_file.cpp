#include <io.h>
#include "catch.hpp"


/*
====================
目录遍历，涉及3个API:
_findfirst
_findnext
_findclose
====================
*/
TEST_CASE("os.walk", "io")
{
	_finddata_t data;
	intptr_t handle = _findfirst("C:\\*", &data);
	REQUIRE(-1 != handle);
	REQUIRE(0 == _findnext(handle, &data));
	_findclose(handle);
}
