/*
===============================================================================
创建时间：2023/12/22
说明：在看pbrt的代码时发现，它里面用TaggedPointer实现了类似继承的“效果”。
===============================================================================
*/

#include <iostream>

class TaggedPointer
{
public:
	template<typename T>
	TaggedPointer(T* ptr)
	{
	}

	//TODO: dispatch
};

class Shape : public TaggedPointer
{
public:
	using TaggedPointer::TaggedPointer;

	void Render()
	{
		//Distach
	}
};

class Sphere
{
public:
	void Render()
	{
		std::cout << "Sphere::Render" << std::endl;
	}
};

int main()
{
	Shape sphere = new Sphere();

	return 0;
}
