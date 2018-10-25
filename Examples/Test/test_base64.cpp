#include <assert.h>
#include <vector>
#include <string>
#include "catch.hpp"

/*
================
base64编码、解码
================
*/

static char base64Char[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";


static std::string base64_encode(void *ptr, size_t sz)
{
	std::string res;
	assert(res.size() % 4 == 0);
	return res;
}


static std::vector<char> base64_decode(std::string const &str)
{
	assert(str.size() % 4 == 0);
	std::vector<char> data;
	return data;
}


TEST_CASE("base64", "base64")
{
#if 0
	srand((unsigned int)time(NULL));

	for (size_t len = 1; len < 100; ++len) {
		std::vector<char> data(len);
		for (auto &c : data) {
			c = rand() % 0xff;
		}

		std::string const str = base64_encode(&data[0], len);
		std::vector<char> const data2 = base64_decode(str);

		REQUIRE(data.size() == data2.size());

		for (size_t i = 0; i < data.size(); ++i) {
			REQUIRE(data[i] == data2[i]);
		}
	}
#endif
}
