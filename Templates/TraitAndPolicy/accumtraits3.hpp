#ifndef ACCUMTRAITS2_HPP
#define ACCUMTRAITS2_HPP

template<typename T>
class AccumTraits;

template<>
class AccumTraits<int>
{
public:
	typedef int Acct;
};

template<>
class AccumTraits<char>
{
public:
	typedef int Acct;
};


//template<>
//class AccumTraits<float>
//{
//public:
//	typedef float Acct;
//};


template<typename T>
inline typename AccumTraits<T>::Acct accum3(T const* beg, T const* end)
{
	typedef typename AccumTraits<T>::Acct TotalType;
	TotalType total = TotalType();
	while (beg != end) {
		total += *beg;
		++beg;
	}
	return total;
}

#endif // ACCUMTRAITS2_HPP
