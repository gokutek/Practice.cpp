#include <iostream>
#include <vector>


// 与auto一起使用
template<typename T1, typename T2>
auto add(T1 const &v1, T2 const &v2) -> decltype(v1 + v2)
{
    return v1 + v2;
}


inline std::string&& test_decltype_ret_str()
{
    return std::string("xxx");
}


void test_decltype()
{
    // decltype(x) 返回的是参数x的类型
    int i= 4;
    decltype(i) j = 10; // j为int
    int x = i + j;
    std::cout << x << std::endl;

    decltype((i)) k = j; // k为int&
    std::cout << "j=" << j << std::endl;
    k = 111;
    std::cout << "j=" << j << std::endl;

    std::vector<int> vec1 = { 1, 2, 3, 4, 5 };
    decltype(vec1.begin()) iter = vec1.begin();
    for (; iter != vec1.end(); ++iter) {
        std::cout << *iter << std::endl;
    }

    // 重用匿名类型
    struct {
        int age;
        float height;
    } info;
    info.age = 1;
    info.height = 2.3f;

    decltype(info) s1;
    s1.age = 22;
    s1.height = 55.66f;

    // 其它例子
    decltype(info.age) hhaa = 111;
    decltype(test_decltype_ret_str()) str = "hahah";
    decltype(info.age++) hhaa11 = 111;      // int
    decltype(++info.age) hhaa12 = hhaa11;   // int&

    // 与auto结合
    int n1 = add(100, 200);
    std::cout << n1 << std::endl;

    //int n2 = add(100, 200.0f); // warning C4244: “初始化”: 从“float”转换到“int”，可能丢失数据
    float n3 = add(100, 200.123f);
    std::cout << n3 << std::endl;
}
