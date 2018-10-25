#include <assert.h>
#include <vector>
#include <string>
#include "catch.hpp"

/*
================
base64编码、解码
================
*/

static char base64Char[65] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";


static std::string base64_encode(void *data, size_t sz)
{
	std::string res;

	uint8_t const *ptr = (uint8_t const*)data;
	while (sz >= 3) {
		res.push_back(base64Char[ptr[0] >> 2]);
		res.push_back(base64Char[((ptr[0] & 3) << 4) | (ptr[1] >> 4)]);
		res.push_back(base64Char[((ptr[1] & 0xf) << 2) | (ptr[2] >> 6)]);
		res.push_back(base64Char[ptr[2] & 0x3f]);
		sz -= 3;
		ptr += 3;
	}

	if (sz == 1) {
		res.push_back(base64Char[ptr[0] >> 2]);
		res.push_back(base64Char[(ptr[0] & 0x3) << 4]);
		res.push_back('=');
		res.push_back('=');
	} else if (sz == 2) {
		res.push_back(base64Char[ptr[0] >> 2]);
		res.push_back(base64Char[((ptr[0] & 3) << 4) | (ptr[1] >> 4)]);
		res.push_back(base64Char[(ptr[1] & 0xf) << 2]);
		res.push_back('=');
	}

	assert(res.size() % 4 == 0);
	return res;
}


static char char_index(char c)
{
	if (c >= 'A' && c <= 'Z') { return c - 'A';	} 
	if (c >= 'a' && c <= 'z') { return c - 'a' + 26; }
	if (c >= '0' && c <= '9') { return c - '0' + 52; }
	if (c == '+') { return 62; }
	if (c == '/') {	return 63; }
	assert(0);
	return -1;
}


static std::vector<char> base64_decode(std::string const &str)
{
	assert(str.size() % 4 == 0);
	std::vector<char> data;
	for (size_t i = 0; i < str.size(); i += 4) {
		char const c0 = str[i];
		char const c1 = str[i + 1];
		char const c2 = str[i + 2];
		char const c3 = str[i + 3];
		if (c3 != '=') {
			char const i0 = char_index(c0);
			char const i1 = char_index(c1);
			char const i2 = char_index(c2);
			char const i3 = char_index(c3);
			char const b0 = (i0 << 2) | (i1 >> 4);
			char const b1 = ((i1 & 0xf) << 4) | ((i2 >> 2) & 0xf);
			char const b2 = ((i2 & 3) << 6) | i3;
			data.push_back(b0);
			data.push_back(b1);
			data.push_back(b2);
		} else if (c2 == '=') {
			char const i0 = char_index(c0);
			char const i1 = char_index(c1);
			char const b0 = ((i0 << 6) | i1) >> 4;
			data.push_back(b0);
		} else {
			char const i0 = char_index(c0);
			char const i1 = char_index(c1);
			char const i2 = char_index(c2);
			char const b0 = (i0 << 2) | (i1 >> 4);
			char const b1 = ((i1 & 0xf) << 4) | (i2 >> 2);
			data.push_back(b0);
			data.push_back(b1);
		}
	}
	return data;
}


TEST_CASE("base64", "base64")
{
	REQUIRE("NA==" == base64_encode("4", 1));
	REQUIRE("NDU=" == base64_encode("45", 2));
	REQUIRE("MTIz" == base64_encode("123", 3));
	REQUIRE("NDU2Nw==" == base64_encode("4567", 4));
	REQUIRE("NDU2Nzg=" == base64_encode("45678", 5));
	
	srand((unsigned int)time(NULL));

	for (size_t len = 1; len < 100; ++len) {
		std::vector<char> data(len);
		for (auto &c : data) {
			c = rand() % 0xff;
		}

		std::string const buf = base64_encode(&data[0], len);
		std::vector<char> const data2 = base64_decode(buf);

		REQUIRE(data.size() == data2.size());

		for (size_t i = 0; i < data.size(); ++i) {
			REQUIRE(data[i] == data2[i]);
		}
	}
}
