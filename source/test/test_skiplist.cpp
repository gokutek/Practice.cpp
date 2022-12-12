/*
 * 2022/07/28: 测试skiplist数据结构
 */

#include <assert.h>
#include <iostream>
#include <memory>
#include <vector>
#include <time.h>
#include <set>
#include <crtdbg.h>

#ifdef _DEBUG
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif // _DEBUG

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

	~skip_list_node()
	{
		free(next_);
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
	~skip_list();
	size_t size() const;
	bool insert(int key, int value);
	int* find(int key) const;
	bool remove(int key);

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

skip_list::~skip_list()
{
	delete head_;
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

bool skip_list::remove(int key)
{
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

	if (!node->next_[0] || node->next_[0]->key_ != key)
	{
		return false;
	}

	skip_list_node* node_to_del = node->next_[0];

	int level = node->next_[0]->level_;
	for (int i = 0; i <= level; ++i)
	{
		per_level_nodes[i]->next_[i] = per_level_nodes[i]->next_[i]->next_[i];
	}

	--count_;

	delete node_to_del;

	return true;
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
	_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF);

	srand((unsigned int)time(nullptr));
	std::set<int> set;

	skip_list list;

	for (int i = 0; i < 20; ++i)
	{
		int key = rand() % INT_MAX;
		list.insert(key, key);

		set.insert(key);
	}

	list.dump();
	std::cout << "---" << std::endl;

	for (int key : set)
	{
		std::cout << *list.find(key) << std::endl;
	}

	for (int key : set)
	{
		bool ret = list.remove(key);
		assert(ret);
	}

	return 0;
}
