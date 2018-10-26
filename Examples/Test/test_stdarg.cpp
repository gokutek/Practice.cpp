#include <stdarg.h>
#include "catch.hpp"


static std::string my_printf(char const *fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	char buffer[512];
	vsprintf(buffer, fmt, args);
	va_end(args);
	return buffer;
}


TEST_CASE("va_list", "stdarg")
{
	std::string const str = my_printf("hello %d %s", 123, "world");
	REQUIRE(str == "hello 123 world");
}
