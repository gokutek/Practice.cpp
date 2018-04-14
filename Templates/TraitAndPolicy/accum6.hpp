#ifndef ACCUM6_HPP
#define ACCUM6_HPP

template<typename T>
struct AccumTraits6;

template<>
struct AccumTraits6<int>
{
	typedef int TRet;

	static TRet zero()
	{
		return 0;
	}
};

template<>
struct AccumTraits6<char>
{
	typedef int TRet;

	static TRet zero()
	{
		return 0;
	}
};


// 加法
struct SumPolicy
{
	template<typename TRet, typename T>
	static void accum(TRet& total, T const& val)
	{
		total += val;
	}
};


// 乘法
struct MulPolicy
{
	template<typename TRet, typename T>
	static void accum(TRet& total, T const& val)
	{
		total *= val;
	}
};


// 把加法操作从算法中分离出来，用户可以传入不同的Policy操作
template<typename T, 
	typename Policy = SumPolicy,
	typename Trait = AccumTraits6<T> >
inline typename Trait::TRet accum6(T const* beg, T const* end)
{
	typename Trait::TRet total = Trait::zero();
	while (beg != end) {
		Policy::accum(total, *beg);
		++beg;
	}
	return total;
}

#endif // ACCUM6_HPP
