/*
 * 2023/03/20:
 */

#include <iostream>
#include <assert.h>

static void some_func(const char* str)
{
	std::cout << str << std::endl;
}

int main()
{
	some_func(std::string("123").c_str()); //������ô�á�

	//����ķ�ʽ�Ǵ���ģ������������ڴ��ͷţ�Ұָ��
#if 0
	const char* s = std::string("124").c_str();
	std::cout << s << std::endl;
#endif
	return 0;
}
