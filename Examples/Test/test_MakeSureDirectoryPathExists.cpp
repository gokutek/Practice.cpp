#include <string>
#include <direct.h>
#include "catch.hpp"


static void MakeSureDirectoryPathExists(std::string const &dir)
{
    char *p = (char*)dir.c_str();
    while (*p)
    {
        char *x = strchr(p, '\\');
        if (!x) { break; }

        *x = 0;
        _mkdir(dir.c_str());
        p = x + 1;
        *x = '\\';
    }
}


TEST_CASE("MakeSureDirectoryPathExists", "[MakeSureDirectoryPathExists]")
{
}
