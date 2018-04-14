#include <iostream>
#include "accum1.hpp"
#include "accumtraits3.hpp"
#include "accum5.hpp"
#include "accum6.hpp"
#include "sumpolicy2.hpp"
#include "sizeof.hpp"
#include "elementtype.hpp"
#include "isclasst.hpp"
#include "apply1.hpp"
#include "ifthenelse.hpp"


int main()
{
	int num[] = { 1, 2, 3, 4, 5 };
	std::cout << accum1(num, num + 5) << std::endl;

	// 当T被推导为char时，total在内部累加时会溢出,
	char name[] = "abcd";
	std::cout << accum1(name, name + strlen(name)) << std::endl;

	// 上面问题的一种解决方案是，增加一个模板参数，由用户传入这个类型参数作为total变量的类型
	std::cout << "- test: accum2" << std::endl;
	std::cout << accum2<int>(name, name + strlen(name)) << std::endl;

	// 比较好的办法，由traits推导返回值类型，不需要用户自己传
	std::cout << "- test: accum3" << std::endl;
	std::cout << accum3(num, num + 5) << std::endl;
	std::cout << accum3(name, name + strlen(name)) << std::endl;

	// 没有AccumTraits<float>特化，所以下面的代码编译失败
	//float fNum[] = { 1.0f, 2.0f, 3.0f, 4.0f, 5.0f };
	//std::cout << accum3(fNum, fNum + 5) << std::endl;

	std::cout << "- test: accum5" << std::endl;
	std::cout << accum5(num, num + 5) << std::endl;
	std::cout << accum5(name, name + strlen(name)) << std::endl;

	std::cout << "- test: accum6" << std::endl;
	std::cout << accum6(num, num + 5) << std::endl;
	std::cout << accum6<int, MulPolicy>(num, num + 5) << std::endl; // 输出了0，乘法的初值应该是1

	std::cout << "- test: accum7" << std::endl;
	std::cout << accum7(num, num + 5) << std::endl;

	std::cout << "- test: accum8" << std::endl;
	std::cout << accum8(num, num + 5) << std::endl;

	std::cout << "- test: accum0" << std::endl;
	std::cout << accum8(num, num + 5) << std::endl;

	std::cout << "- test: TypeSize" << std::endl;
	std::cout << TypeSize<char>::value << std::endl;
	std::cout << TypeSize<int>::value << std::endl;
	std::cout << TypeSize<int64_t>::value << std::endl;

	std::cout << "- test: print_element_type" << std::endl;
	print_element_type(std::vector<int>());
	print_element_type(std::list<float>());

	std::cout << "- test: IsClassT" << std::endl;
	std::cout << IsClassT<int>::Yes << std::endl;
	std::cout << IsClassT<std::string>::Yes << std::endl;
	std::cout << IsClassT<std::vector<int> >::Yes << std::endl;

	struct Foo {};
	std::cout << IsClassT<Foo>::Yes << std::endl;
	
	enum Bar {};
	std::cout << IsClassT<Bar>::Yes << std::endl;

	UnitTest_Apply1();

	return 0;
}
