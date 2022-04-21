#include <queue>
#include <vector>
#include <algorithm>
#include <functional>
#include "catch.hpp"


TEST_CASE("MinHeap", "priority_queue")
{
	std::priority_queue<int, std::vector<int>, std::greater<int> > pqInt;
	pqInt.push(1);
	pqInt.push(2);
	pqInt.push(3);
	pqInt.push(4);
	pqInt.push(5);

	REQUIRE(pqInt.top() == 1);
	pqInt.pop();

	REQUIRE(pqInt.top() == 2);
	pqInt.pop();

	REQUIRE(pqInt.top() == 3);
	pqInt.pop();

	REQUIRE(pqInt.top() == 4);
	pqInt.pop();

	REQUIRE(pqInt.top() == 5);
	pqInt.pop();
}


TEST_CASE("MaxHeap", "priority_queue")
{
	std::priority_queue<int> pqInt;
	pqInt.push(1);
	pqInt.push(2);
	pqInt.push(3);
	pqInt.push(4);
	pqInt.push(5);

	REQUIRE(pqInt.top() == 5);
	pqInt.pop();

	REQUIRE(pqInt.top() == 4);
	pqInt.pop();

	REQUIRE(pqInt.top() == 3);
	pqInt.pop();

	REQUIRE(pqInt.top() == 2);
	pqInt.pop();

	REQUIRE(pqInt.top() == 1);
	pqInt.pop();
}
