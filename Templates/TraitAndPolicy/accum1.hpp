#ifndef ACCUM1_HPP
#define ACCUM1_HPP

template<typename T>
inline T accum1(T const* beg, T const* end)
{
	T total = T(); // 假设T()事实上会生成一个等于0的值
	while (beg != end) {
		total += *beg;
		++beg;
	}
	return total;
}


template<typename TRet, typename T>
inline TRet accum2(T const* beg, T const* end)
{
	TRet total = TRet();
	while (beg != end) {
		total += *beg;
		++beg;
	}
	return total;
}

#endif // ACCUM1_HPP
