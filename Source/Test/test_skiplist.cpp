#include <iostream>
#include <memory>
#include <vector>

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
	void remove(int key);
	int* find(int key);

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

void skip_list::remove(int key)
{
}

int* skip_list::find(int key)
{
	return nullptr;
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
	skip_list list;
	list.insert(1, 1);
	list.insert(10, 10);
	list.insert(3, 3);
	list.insert(8, 8);
	list.insert(9, 9);
	list.insert(90, 90);
	list.insert(70, 70);
	list.insert(850, 850);
	list.insert(630, 630);
	list.insert(40, 40);
	list.insert(40, 40);

	list.dump();

	return 0;
}
