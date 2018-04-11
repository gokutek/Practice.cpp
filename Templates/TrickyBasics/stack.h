#ifndef __stack_h__
#define __stack_h__

#include <vector>

template<typename T, template<typename ELEM, typename ALLOC = std::allocator<ELEM> > class CONT = std::vector>
class Stack
{
public:
	void push(T const &elem);
	void pop();
	T top() const;

	bool empty() const
	{
		return elements_.empty();
	}


	template<typename T2, template<typename ELEM2, typename ALLOC = std::allocator<ELEM2> > class CONT2 = std::vector>
	Stack<T, CONT>& operator=(Stack<T2, CONT2> const &other); // 返回值类型可以简化为Stack&

private:
	CONT<T> elements_;
};


template<typename T, template<typename, typename> class CONT>
inline void Stack<T, CONT>::push(T const& elem)
{
	elements_.push_back(elem);
}


template<typename T, template<typename, typename> class CONT>
inline void Stack<T, CONT>::pop()
{
	elements_.pop_back();
}


template<typename T, template<typename, typename> class CONT>
inline T Stack<T, CONT>::top() const
{
	return elements_.back();
}


// 成员函数又是模板，要写两行
template<typename T, template<typename, typename> class CONT>
template<typename T2, template<typename, typename> class CONT2>
Stack<T, CONT>& Stack<T, CONT>::operator=(Stack<T2, CONT2> const &other)
{
	elements_.clear();
	auto tmp = other;
	while (!tmp.empty()) {
		push(tmp.top());
		tmp.pop();
	}
	return *this;
}

#endif // __stack_h__
