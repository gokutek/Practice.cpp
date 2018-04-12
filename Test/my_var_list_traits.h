#ifndef __my_var_list_traits_h__
#define __my_var_list_traits_h__

#include <assert.h>
#include "my_var_list.h"

template<typename T>
struct TArgTraits
{
	enum { Type = VarType::ARG_TYPE_UNKNOWN };
};


template<>
struct TArgTraits<int>
{
	enum { Type = VarType::ARG_TYPE_INT };
};


template<>
struct TArgTraits<float>
{
	enum { Type = VarType::ARG_TYPE_FLOAT };
};


template<>
struct TArgTraits<double>
{
	enum { Type = VarType::ARG_TYPE_DOUBLE };
};



template<typename TYPE>
struct TVarGetter
{
	static TYPE Get(const MyVarList& vl, size_t index)
	{
		assert(0);
	}
};


template<>
struct TVarGetter<int>
{
	static int Get(const MyVarList& vl, size_t index)
	{
		return vl.IntVal(index);
	}
};


template<>
struct TVarGetter<float>
{
	static float Get(const MyVarList& vl, size_t index)
	{
		return vl.FloatVal(index);
	}
};


template<>
struct TVarGetter<double>
{
	static double Get(const MyVarList& vl, size_t index)
	{
		return vl.DoubleVal(index);
	}
};

#endif // __my_var_list_traits_h__
