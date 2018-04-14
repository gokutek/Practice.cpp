#ifndef ACCUM5_HPP
#define ACCUM5_HPP

template<typename T>
struct AccumTraits5;

template<>
struct AccumTraits5<int>
{
	typedef int TRet;
	
	static TRet zero()
	{
		return 0;
	}
};

template<>
struct AccumTraits5<char>
{
	typedef int TRet;

	static TRet zero()
	{
		return 0;
	}
};


template<typename T, typename Trait = AccumTraits5<T> >
inline typename Trait::TRet accum5(T const* beg, T const* end)
{
	typename Trait::TRet total = Trait::zero();
	while (beg != end) {
		total += *beg;
		++beg;
	}
	return total;
}

#endif // ACCUM5_HPP
