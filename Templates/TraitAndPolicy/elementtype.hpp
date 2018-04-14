#ifndef ELEMENTTYPE_HPP
#define ELEMENTTYPE_HPP

#include <vector>
#include <list>
#include <stack>
#include <iostream>

template<typename TContainer>
struct ElementT;

template<typename T>
struct ElementT<std::vector<T> >
{
	typedef T Type;
};

template<typename T>
struct ElementT<std::list<T> >
{
	typedef T Type;
};

template<typename T>
struct ElementT<std::stack<T> >
{
	typedef T Type;
};


template<typename T>
inline void print_element_type(T const&)
{
	std::cout << typeid(ElementT<T>::Type).name() << std::endl;
}

#endif // ELEMENTTYPE_HPP
