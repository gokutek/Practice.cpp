#ifndef SUMPOLICY2_HPP
#define SUMPOLICY2_HPP


template<typename T>
struct AccumTraits7;

template<>
struct AccumTraits7<int>
{
	typedef int TRet;

	static TRet zero()
	{
		return 0;
	}
};

template<>
struct AccumTraits7<char>
{
	typedef int TRet;

	static TRet zero()
	{
		return 0;
	}
};


/*
===============================================================================
与accum6.hpp相比，就是把模板参数从成员函数上移到类上
===============================================================================
*/
template<typename TRet, typename T>
struct SumPolicy7
{
	static void accum(TRet& total, T const& val)
	{
		total += val;
	}
};


// 把加法操作从算法中分离出来，用户可以传入不同的Policy操作
template<typename T,
	template<typename, typename> class Policy = SumPolicy7,
	typename Trait = AccumTraits7<T> >
	inline typename Trait::TRet accum7(T const* beg, T const* end)
{
	typename Trait::TRet total = Trait::zero();
	while (beg != end) {
		Policy<Trait::TRet, T>::accum(total, *beg);
		++beg;
	}
	return total;
}


/*
===============================================================================
与accum7相比：
Policy<Trait::TRet, T>::accum(total, *beg);
改为了：
Policy<Trait, T>::accum(total, *beg);
直接将Trait传给Policy。
===============================================================================
*/
template<typename Tait, typename T>
struct SumPolicy8
{
	static void accum(typename Tait::TRet& total, T const& val)
	{
		total += val;
	}
};


template<typename T,
	template<typename, typename> class Policy = SumPolicy8,
	typename Trait = AccumTraits7<T> >
	inline typename Trait::TRet accum8(T const* beg, T const* end)
{
	typename Trait::TRet total = Trait::zero();
	while (beg != end) {
		Policy<Trait, T>::accum(total, *beg);
		++beg;
	}
	return total;
}


#endif // SUMPOLICY2_HPP
