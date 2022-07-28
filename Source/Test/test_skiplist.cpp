#include <iostream>

struct skip_list_node
{
	skip_list_node(int key, int value, size_t level)
	{
		key_ = key;
		value_ = value;
		level_ = level;

		size_t sz = sizeof(skip_list_node*) * level;
		next_ = (skip_list_node**)malloc(sz);
		memset(next_, 0, sz);
	}

	int key_;
	int value_;
	size_t level_;
	skip_list_node** next_;
};

class skip_list
{
public:
	skip_list(size_t max_level);
	size_t size() const;
	void insert(int key, int value);
	void remove(int key);
	int* find(int key);

private:

private:
	const size_t max_level_;
	size_t count_;
	size_t cur_level_;
	skip_list_node* head_;
};

skip_list::skip_list(size_t max_level = 8)
	: max_level_(max_level)
	, count_(0)
	, cur_level_(0)
{
	head_ = new skip_list_node(-1, -1, max_level_);
}

size_t skip_list::size() const
{
	return count_;
}

void skip_list::insert(int key, int value)
{
}

void skip_list::remove(int key)
{
}

int* skip_list::find(int key)
{
	return nullptr;
}

int main()
{	
	skip_list list;
	list.insert(1, 10);
	list.insert(10, 100);
	list.insert(3, 30);
	list.insert(8, 80);

	return 0;
}
