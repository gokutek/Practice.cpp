#include <queue>
#include <vector>
#include <iostream>
#include <algorithm>
#include <functional>

struct Edge
{
	size_t	v_;
	size_t	w_;
	float	weight_;

	Edge(size_t v, size_t w, float weight)
		: v_(v)
		, w_(w)
		, weight_(weight)
	{
	}

	// 必须实现
	bool operator<(Edge const &other) const
	{
		return weight_ < other.weight_;
	}

	Edge(Edge const &other)
	{
		std::cout << __FUNCTION__ << std::endl;
		this->v_ = other.v_;
		this->w_ = other.w_;
		this->weight_ = other.weight_;
	}
};


int main()
{
	std::priority_queue<Edge> pq;
	pq.push(Edge(0, 0, 1));
	pq.push(Edge(0, 0, 2));
	pq.push(Edge(0, 0, 3));
	pq.push(Edge(0, 0, 4));
	pq.push(Edge(0, 0, 5));

	while (!pq.empty()) {
		std::cout << pq.top().weight_ << std::endl;
		pq.pop();
	}

	//std::priority_queue<int> pqInt; // 从大到小
	std::priority_queue<int, std::vector<int>, std::greater<int> > pqInt; // 从小到大
	pqInt.push(1);
	pqInt.push(2);
	pqInt.push(3);
	pqInt.push(4);
	pqInt.push(5);
	while (!pqInt.empty())
	{
		std::cout << pqInt.top() << std::endl;
		pqInt.pop();
	}
	return 0;
}
