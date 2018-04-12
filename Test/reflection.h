#ifndef REFLECTION_H
#define REFLECTION_H

#include <vector>
#include <assert.h>
#include "my_var_list.h"
#include "my_var_list_traits.h"

namespace reflection
{
	template<typename _Class, typename _Rt, typename... Args>
	struct RTTIFuncType
	{
		typedef _Rt(_Class::*pmfn_t)(Args...);

		RTTIFuncType(pmfn_t pmfn)
		{
			argTypes_ = { TArgTraits<Args>::Type... };
			pmfn_ = pmfn;
		}

		template<typename T>
		T extractArg(const MyVarList& args, size_t index)
		{
			return TVarGetter<T>::Get(args, index);
		}

		void invoke(_Class* obj, const MyVarList& args, MyVarList& res)
		{
			assert(sizeof...(Args) == args.GetCount() && "invalid argc");
			size_t index = args.GetCount() - 1;
			(obj->*pmfn_)(extractArg<Args>(args, index--)...);
		}

		std::vector<int>	argTypes_;	///< 每个参数的数据类型
		pmfn_t				pmfn_;		///< 成员函数指针
	};


	class ReflectionTestClass
	{
	public:
		void func0()
		{
			std::cout << __FUNCTION__ << std::endl;
		}

		void func1(int nval) 
		{
			std::cout << __FUNCTION__ << ":" << nval << std::endl;
		}

		void func2(int nval, float fval) 
		{
			std::cout << __FUNCTION__ << ":" << nval << ", " << fval << std::endl;
		}

		void func3(int nval, float fval, double dval)
		{
			std::cout << __FUNCTION__ << ":" << nval << ", " << fval << ", " << dval << std::endl;
		}
	};


	inline void reflection_unite_test()
	{
		MyVarList foo;
		ReflectionTestClass obj;

		RTTIFuncType<ReflectionTestClass, void> v0(&ReflectionTestClass::func0);
		v0.invoke(&obj, foo, foo);

		RTTIFuncType<ReflectionTestClass, void, int> v1(&ReflectionTestClass::func1);
		MyVarList v1args;
		v1args.AddInt(12);
		v1.invoke(&obj, v1args, foo);

		RTTIFuncType<ReflectionTestClass, void, int, float> v2(&ReflectionTestClass::func2);
		MyVarList v2args;
		v2args.AddInt(123);
		v2args.AddFloat(252.365f);
		v2.invoke(&obj, v2args, foo);

		RTTIFuncType<ReflectionTestClass, void, int, float, double> v3(&ReflectionTestClass::func3);
		MyVarList v3args;
		v3args.AddInt(555);
		v3args.AddFloat(454.111f);
		v3args.AddDouble(1123.11444129);
		v3.invoke(&obj, v3args, foo);
		return;
	}

} // end of namespace: reflection

#endif // REFLECTION_H
