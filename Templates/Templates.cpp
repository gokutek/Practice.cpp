#include <sstream>
#include <string>
#include <iostream>

template<typename T>
std::string debug_rep(const T& t)
{
	std::ostringstream oss;
	oss << t;
	return oss.str();
}


template<typename T>
std::string debug_rep(T* p)
{
	std::ostringstream oss;
	oss << "pointer: " << p;
	if (p) {
		oss << " " << debug_rep(*p);
	} else {
		oss << " null pointer";
	}
	return oss.str();
}


std::string debug_rep(const std::string& s)
{
	return "\"" + s + "\"";
}


std::string debug_rep(const char* s)
{
	return debug_rep(std::string(s));
}


std::string debug_rep(char* s)
{
	return debug_rep(std::string(s));
}


int main()
{
	std::string str = "hi";
	
	std::cout << debug_rep(str) << std::endl;
	
	std::cout << debug_rep(&str) << std::endl;

	const std::string* pStr = &str;
	std::cout << debug_rep(pStr) << std::endl;

	std::cout << debug_rep("hello world") << std::endl;

	return 0;
}
