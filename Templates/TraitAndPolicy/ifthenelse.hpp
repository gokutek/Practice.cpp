#ifndef IF_THEN_ELSE_HPP
#define IF_THEN_ELSE_HPP

template<bool cond, typename T1, typename T2>
struct IfThenElse;

// 局部特化，true的话选择T1
template<typename T1, typename T2>
struct IfThenElse<true, T1, T2>
{
	typedef T1 ResultT;
};

// 局部特化，false的话选择T2
template<typename T1, typename T2>
struct IfThenElse<false, T1, T2>
{
	typedef T2 ResultT;
};

#endif // IF_THEN_ELSE_HPP
