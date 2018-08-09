#ifndef __serialization_h__
#define __serialization_h__

#include <vector>
#include <tuple>
#include <string>

/*
===============================================================================
===============================================================================
*/

#define BOOST_PP_CAT_I(a, b) a##b

#define BOOST_PP_CAT(a, b) BOOST_PP_CAT_I(a, b)

#define BOOST_PP_VARIADIC_SIZE(_1, _2, _3, _4, _5, size, ...) size


#define ARG_NAME_1(arg) #arg
#define ARG_NAME_2(arg, ...) #arg, ARG_NAME_1(__VA_ARGS__)
#define ARG_NAME_3(arg, ...) #arg, ARG_NAME_2(__VA_ARGS__)
#define ARG_NAME_4(arg, ...) #arg, ARG_NAME_3(__VA_ARGS__)
#define ARG_NAME_5(arg, ...) #arg, ARG_NAME_4(__VA_ARGS__)

#define MACRO_EXPAND(...)                 __VA_ARGS__

// 不用MACRO_EXPAND包一层也不能正常工作
#define APPLY_VARIADIC_MACRO(macro, ...)  MACRO_EXPAND(macro(__VA_ARGS__))


/*
===============================================================================
不用BOOST_PP_CAT包一下结果不正确，可能是vc编译器的BUG。
参数列表为空时，返回的也是1。
===============================================================================
*/
#define SERIALIZATION_PP_VARIADIC_SIZE(...) BOOST_PP_CAT(BOOST_PP_VARIADIC_SIZE(##__VA_ARGS__, 5, 4, 3, 2, 1, 0),)

#define SERIALIZATION(cls, ...)\
inline std::vector<std::string> GetSerialiation()\
{\
    return std::vector<std::string>{ APPLY_VARIADIC_MACRO(BOOST_PP_CAT(ARG_NAME_, SERIALIZATION_PP_VARIADIC_SIZE(__VA_ARGS__)), __VA_ARGS__) };\
}

#endif // __serialization_h__
