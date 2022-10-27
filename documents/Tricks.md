## const

```C++
const int* ip;    // ip is a pointer to const int
int const* ip;    // ip is a pointer to int const
int* const ip;    // ip is a const pointer to int

int x = 100;
int & const f = x; // warning C4227: 使用了记时错误: 忽略引用上的限定符
```

const是不能放在&后面的，因为引用本身永远都是无法修改的

## DLL与EXE因编译选项不一样而崩溃
DLL是使用MT选项编译生成的，导出的接口返回了一个std::string，使用该DLL的用户程序是使
用MD编译的，运行时调用这个接口会崩溃。

## 参考资料
[全局变量初始化顺序问题](http://blog.chinaunix.net/uid-25885064-id-3063983.html)
