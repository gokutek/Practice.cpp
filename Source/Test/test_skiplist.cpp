#include <iostream>
#include <memory>
#include <vector>
#include <time.h>
#include <set>

struct skip_list_node
{
	skip_list_node(int key, int value, int level)
	{
		key_ = key;
		value_ = value;
		level_ = level;

		int sz = sizeof(skip_list_node*) * (level+1);
		next_ = (skip_list_node**)malloc(sz);
		memset(next_, 0, sz);
	}

	int key_;
	int value_;
	int level_;
	skip_list_node** next_;
};

class skip_list
{
public:
	skip_list(int max_level);
	size_t size() const;
	bool insert(int key, int value);
	int* find(int key) const;
	void remove(int key);

	void dump() const;

private:

private:
	const int max_level_;
	int cur_level_;
	size_t count_;
	skip_list_node* head_;
};

skip_list::skip_list(int max_level = 8)
	: max_level_(max_level)
	, cur_level_(0)
	, count_(0)
{
	head_ = new skip_list_node(-1, -1, max_level_);
}

size_t skip_list::size() const
{
	return count_;
}

bool skip_list::insert(int key, int value)
{
	//每层的指针域?
	std::vector<skip_list_node*> per_level_nodes(max_level_, nullptr);
	skip_list_node* node = head_;
	for (int level = (int)cur_level_; level >= 0; --level)
	{
		while (node->next_[level] && node->next_[level]->key_ < key)
		{
			node = node->next_[level];
		}

		per_level_nodes[level] = node;
	}

	if (node->next_[0] && node->next_[0]->key_ == key)
	{
		return false;
	}

	//这个节点的高度?
	int insert_level = rand() % max_level_;
	if (insert_level > cur_level_) 
	{
		insert_level = ++cur_level_;
		per_level_nodes[insert_level] = head_;
	}

	skip_list_node* new_node = new skip_list_node(key, value, insert_level);
	for (int level = insert_level; level >= 0; --level)
	{
		skip_list_node* node = per_level_nodes[level];
		new_node->next_[level] = node->next_[level];
		node->next_[level] = new_node;
	}

	++count_;

	return true;
}

int* skip_list::find(int key) const
{
	//每层的指针域?
	std::vector<skip_list_node*> per_level_nodes(max_level_, nullptr);
	skip_list_node* node = head_;
	for (int level = (int)cur_level_; level >= 0; --level)
	{
		while (node->next_[level] && node->next_[level]->key_ < key)
		{
			node = node->next_[level];
		}

		per_level_nodes[level] = node;
	}

	if (node->next_[0] && node->next_[0]->key_ == key)
	{
		return &node->next_[0]->value_;
	}

	return nullptr;
}

void skip_list::remove(int key)
{
}

void skip_list::dump() const
{
	for (skip_list_node* node = head_->next_[0]; node; node = node->next_[0])
	{
		std::cout << node->value_ << std::endl;
	}
}

int main()
{
	srand((unsigned int)time(nullptr));
	std::set<int> set;

	skip_list list;

	for (int i = 0; i < 20; ++i)
	{
		int key = rand() % INT_MAX;
		list.insert(key, key);

		set.insert(key);
	}

	for (int key : set)
	{
		std::cout << *list.find(key) << std::endl;
	}

	std::cout << std::endl;

	list.dump();

	return 0;
}
