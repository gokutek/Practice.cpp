#include <array>
#include <vector>
#include <string>
#include <set>
#include <queue>
#include <deque>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <iostream>
#include "kapok/Kapok.hpp"
#include <boost/timer.hpp>
#include "test_kapok.hpp"

void test()
{
    kapok::Serializer sr;
    kapok::DeSerializer dr;

	auto tp = std::make_tuple(10, 12, std::string("test"));
	sr.Serialize(tp, "tuple");
	dr.Parse(sr.GetString());
	std::tuple<int, int, std::string> p;
	dr.Deserialize(p, "tuple");
}

template<class T>
void test_array( T& arr)
{
	using namespace kapok;
	Serializer sr;
	sr.Serialize(arr, "test");
	std::cout << sr.GetString() << std::endl;

	DeSerializer dr;
	dr.Parse(sr.GetString());
	T de_arr;
	dr.Deserialize(de_arr, "test");
}

template<class T>
void test_map( T& map)
{
	using namespace kapok;
	Serializer sr;
	sr.Serialize(map, "test");
	std::cout << sr.GetString() << std::endl;

	DeSerializer dr;
	dr.Parse(sr.GetString());
	T de_map;
	dr.Deserialize(de_map, "test");
}

struct person 
{
	int age;
	std::string name;
	//META(age, name);
	META(age, name);
};

template<class T>
void test_object(T& t)
{
	using namespace kapok;
	Serializer sr;
	sr.Serialize(t, "test");
	std::cout << sr.GetString() << std::endl;

	DeSerializer dr;
	dr.Parse(sr.GetString());
	T de_t;
	dr.Deserialize(de_t, "test");
}

struct test_p
{
	int a;
	std::string b;
	std::map<std::string, int> c;
	person d;
	META(a, b, c, d);

};

struct complex_t
{
	int a;
	std::string b;
	std::map<std::string, person> c;
	std::map<std::string, std::vector<person>> d;
	std::vector<std::map<std::string, std::vector<person>>> e;
	META(a, b, c, d, e);

};

struct complex_t1
{
	int a;
	std::map<std::string, std::array<person, 2>> d;

	META(a, d);

};

void test_recurse1()
{
	using namespace kapok;
	complex_t1 cpl = { 1,{ { { "d",{ { { 20, "tom" },{ 20, "tom" } } } } } } };
	Serializer sr;
	sr.Serialize(cpl, "test");
	std::string str = sr.GetString();
	std::cout << str << std::endl;

	DeSerializer dr;
	dr.Parse(sr.GetString());
	complex_t1 de_t;
	dr.Deserialize(de_t, "test");
}

void test_recurse()
{
	using namespace kapok;
	complex_t cpl = { 1,"b", {{"c", {20, "tom"}}}, /*{{"d", {{{20, "tom"},{ 20, "tom" } }}}}, */{ { { "d",{ { { 20, "tom" },{ 20, "tom" } } } } } } };
	Serializer sr;
	sr.Serialize(cpl, "test");
	std::string str = sr.GetString();
	std::cout << str << std::endl;

	DeSerializer dr;
	dr.Parse(sr.GetString());
	complex_t de_t;
	dr.Deserialize(de_t, "test");
}

void test_new_meta()
{
	using namespace kapok;
	std::map<std::string, int> mp = { {"aa", 2} };
	test_p p = { 20, "test", {{ "aa", 2 }}, {12, "td"} };

	//person p = { 20, "test" };

	Serializer sr;
	sr.Serialize(p, "test");
	std::string str = sr.GetString();
	std::cout << str << std::endl;

	DeSerializer dr;
	dr.Parse(sr.GetString());
	test_p de_t;
	dr.Deserialize(de_t, "test");
}

void test_performance()
{
	using namespace kapok;
	person p = { 20, "test" };
	Serializer sr;
	
	const int LEN = 1000000;
	boost::timer timer;
	for (size_t i = 0; i < LEN; i++)
	{
		sr.Serialize(p, "test");
	}
	std::cout << timer.elapsed() << std::endl;
	
	DeSerializer dr;
	dr.Parse(sr.GetString());
	timer.restart();
	for (size_t i = 0; i < LEN; i++)
	{
		person de_t;
		dr.Deserialize(de_t, "test");
	}
	std::cout << timer.elapsed() << std::endl;
}

void test_tuple()
{
	using namespace std;
	using namespace kapok;
	cout << is_basic_type<int>::value << endl;
	cout << is_normal_class<int>::value << endl;
	std::vector<int> v = { 1,2,3 };
	std::tuple<vector<int>, int> tp = std::make_tuple(v, 2);
	std::map<int, int> mp = { {1,2},{3,4} };
	Serializer sr;
	sr.Serialize(tp);
	std::cout << sr.GetString() << std::endl;

	DeSerializer dr;
	dr.Parse(sr.GetString());
	std::tuple<vector<int>, int> tp1;
	std::map<int, int> mp1;
	dr.Deserialize(tp1);
}

struct my_person
{
	std::vector<int> a;
	int b;
	META(a, b);
};

void test_myperson()
{
	using namespace kapok;
	my_person p = { {1,2,3}, 4 };
	Serializer sr;
	sr.Serialize(p);
	std::cout << sr.GetString() << std::endl;

	DeSerializer dr;
	dr.Parse(sr.GetString());
	my_person p1;
	dr.Deserialize(p1);
}

struct T
{
	int a;
	char b;

	META(a, b);
};

void test_simple()
{
	using namespace kapok;
	Serializer sr;
	sr.Serialize((std::array<int, 5>{1, 2, 3, 4, 5}));
	std::cout << sr.GetString() << std::endl;

	T t{ 66, 'c' };
	sr.Serialize(t, "T");
	std::cout << sr.GetString() << std::endl;
}

void test_array()
{
	using namespace kapok;
	DeSerializer dr;
	dr.Parse(R"({"array":[1,2,3,4,5]})");
	std::array<int, 5> v;
	std::array<int, 5> a{ 1, 2, 3, 4, 5 };
	dr.Deserialize(v, "array");
}

void test_const(const person& p)
{
	using namespace kapok;
	Serializer sr;
	sr.Serialize(p);
	std::cout << sr.GetString() << std::endl;
}

void test_stack()
{
	using namespace kapok;
	DeSerializer dr;
	dr.Parse(R"({"stack":[1,2,3,4,5]})");
	std::stack<int> v;
	dr.Deserialize(v, "stack");
	
	for (int i = 0; i < 5; ++i)
	{
		if(v.top()==i+1)
		std::cout << "ok" << std::endl;
		v.pop();
	}
}

struct messager
{
	int code;
	int id;
	META(code, id);
};

void test_sr()
{
    using namespace kapok;
	messager m = { 1,2 };
	Serializer sr;
	sr.Serialize(m);
	std::cout << sr.GetString() << std::endl;
}

void test_str()
{
	using namespace kapok;
	Serializer sr;
	sr.Serialize("hello, world", "string value");
	std::cout << sr.GetString() << std::endl;
}

void test_optional()
{
	using namespace std::string_literals;
	using namespace kapok;

	struct test_optional_struct
	{
		int a;
		float b;
		boost::optional<std::string> str;
		META(a, b, str);
	};

	// init without init test_optional_struct::str
	boost::optional<test_optional_struct> to_sr = test_optional_struct{ 1, 1.0f };
	Serializer sr;
	sr.Serialize(to_sr);
	std::cout << sr.GetString() << std::endl;

	DeSerializer ds;
	ds.Parse(sr.GetString());
	boost::optional<test_optional_struct> to_ds;
	ds.Deserialize(to_ds);
	bool test_pass =
		to_ds->a == to_sr->a &&
		to_ds->b == to_sr->b &&
		to_ds->str == to_sr->str;

	//full init
	to_sr = test_optional_struct{ 3, 2.0f, "hello optional"s };
	sr.Serialize(to_sr);
	std::cout << sr.GetString() << std::endl;

	ds.Parse(sr.GetString());
	to_ds.reset();
	ds.Deserialize(to_ds);
	test_pass =
		to_ds->a == to_sr->a &&
		to_ds->b == to_sr->b &&
		to_ds->str == to_sr->str;
}

enum class foo_enum
{
	hahahaha = 0,
	jinkela = 1,
};

void test_enum()
{
	using namespace kapok;
	{
		Serializer sr;
		foo_enum aha = foo_enum::hahahaha;
		sr.Serialize(aha);
		std::cout << sr.GetString() << std::endl;
		DeSerializer ds;
		ds.Parse(sr.GetString());
		foo_enum r;
		ds.Deserialize(r);

		assert(r == aha);
	}

	struct test_enum_t
	{
		foo_enum a;
		int b;
		META(a, b);
	};
	
	{
		Serializer sr;
		test_enum_t foo = { foo_enum::jinkela, 8 };
		sr.Serialize(foo);
		std::cout << sr.GetString() << std::endl;
		DeSerializer ds;
		ds.Parse(sr.GetString());
		test_enum_t r;
		ds.Deserialize(r);

		assert(r.a == foo.a && r.b == foo.b);
	}
}

void test_pair()
{
	using namespace kapok;
	std::pair<int, double> p = { 1, 0.2 };
	std::pair<int, double> r;

	Serializer sr;
	sr.Serialize(p);
	auto result = sr.GetString();
	std::cout << result << std::endl;

	DeSerializer ds;
	ds.Parse(result);
	ds.Deserialize(r);

	assert(p.first == r.first && p.second == r.second);
}

void test_variant()
{
	using namespace kapok;
	Serializer sr;

	variant<int, double, std::pair<float, std::string>> a, b;
	a = std::make_pair(6.28, "zhengwei");
	sr.Serialize(a);
	auto result = sr.GetString();
	std::cout << result << std::endl;
	
	DeSerializer ds;
	ds.Parse(result);
	ds.Deserialize(b);

	auto r = boost::get<std::pair<float, std::string>>(a) == boost::get<std::pair<float, std::string>>(b);
	assert(r);
}

TEST_CASE(example)
{
	test_pair();

	test_sr();
	test_str();
	test_stack();
	person p1 = { 20, "aa" };
	test_const(p1);
	test_array();
	test_simple();
	test_myperson();
//	test_performance();
	test_tuple();
	test_recurse1();
	test_recurse();
	test_new_meta();

	std::array<std::string, 5> arr = { "a","b","c", "d", "e" };
	test_array(arr);

	std::vector<std::string> vt = { "a","b","c", "d", "e" };
	test_array(vt);

	int int_arr[5] = { 1,2,3,4,5 };
	test_array(int_arr);

	std::string str_arr[5] = { "a","b","c", "d", "e" };
	test_array(str_arr);

	std::queue<std::string> que_arr;
	que_arr.emplace("a");
	que_arr.emplace("b");
	test_array(que_arr);

	std::deque<std::string> deque_arr;
	deque_arr.push_back("a");
	deque_arr.push_back("b");
	test_array(deque_arr);

	std::priority_queue<std::string> priodeque_arr;
	priodeque_arr.push("a");
	priodeque_arr.push("b");
	test_array(priodeque_arr);

	std::set<std::string> set_arr;
	set_arr.emplace("a");
	set_arr.emplace("b");
	test_array(set_arr);
	std::multiset<std::string> multiset_arr;
	multiset_arr.emplace("a");
	multiset_arr.emplace("b");
	test_array(multiset_arr);
	std::unordered_set<std::string> unordset_arr;
	unordset_arr.emplace("a");
	unordset_arr.emplace("b");
	test_array(unordset_arr);

	std::stack<std::string> stack_arr;
	stack_arr.emplace("a");
	stack_arr.emplace("b");
	test_array(stack_arr);

	std::map<int, std::string> map = { {1, "a"},{2, "b"} };
	test_map(map);
	std::multimap<int, std::string> multimap = { { 1, "a" },{ 2, "b" } };
	test_map(multimap);
	std::unordered_map<int, std::string> unodermap = { { 1, "a" },{ 2, "b" } };
	test_map(unodermap);

	std::map<std::string, int> map1 = { { "a", 1 },{ "b", 2 } };
	test_map(map1);

	person p = { 20, "test" };
	test_object(p);

	std::map<std::string, person> map2= { {"a", p} };
	test_map(map2);

	test_optional();
	test_enum();
	test_variant();
}
