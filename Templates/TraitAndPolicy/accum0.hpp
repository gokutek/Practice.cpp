#ifndef ACCUM0_HPP
#define ACCUM0_HPP

#include <iterator>

template<typename Iter>
inline typename std::iterator_traits<Iter>::value_type accum0(Iter start, Iter end)
{
	typedef typename std::iterator_traits<Iter>::value_type VT;
	VT total = VT();
	while (start != end) {
		total += *start;
		++start;
	}
	return total;
}

#endif // ACCUM0_HPP
