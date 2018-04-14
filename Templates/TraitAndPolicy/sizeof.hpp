#ifndef SIZEOF_HPP
#define SIZEOF_HPP

#include <stddef.h>

template<typename T>
struct TypeSize
{
	static size_t const value = sizeof(T);
};

#endif // SIZEOF_HPP
