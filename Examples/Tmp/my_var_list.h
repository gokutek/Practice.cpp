#ifndef __my_var_list_h__
#define __my_var_list_h__

#include <vector>
#include <string>
#include <assert.h>

enum class VarType
{
	ARG_TYPE_INT,
	ARG_TYPE_FLOAT,
	ARG_TYPE_DOUBLE,
	ARG_TYPE_STRING,
	ARG_TYPE_UNKNOWN
};


struct MyVarList
{
	struct variant
	{
		int type;

		union {
			int nval;
			float fval;
			double dval;
			char str[1024];
		};
	};

	void AddInt(int v);

	void AddFloat(float v);

	void AddDouble(double v);

	void AddString(const std::string& str);

	int IntVal(size_t index) const;

	float FloatVal(size_t index) const;

	double DoubleVal(size_t index) const;

	std::string StringVal(size_t index) const;

	size_t GetCount() const;

private:
	std::vector<variant> vars_;
};


inline void MyVarList::AddInt(int v)
{
	variant var;
	var.nval = v;
	vars_.push_back(var);
}


inline void MyVarList::AddFloat(float v)
{
	variant var;
	var.fval = v;
	vars_.push_back(var);
}


inline void MyVarList::AddDouble(double v)
{
	variant var;
	var.dval = v;
	vars_.push_back(var);
}


inline void MyVarList::AddString(const std::string& str)
{
	assert(str.size() < sizeof(variant));
	variant var;
	memcpy(&var.str[0], str.c_str(), str.size() + 1);
	vars_.push_back(var);
}


inline int MyVarList::IntVal(size_t index) const
{
	return vars_[index].nval;
}


inline float MyVarList::FloatVal(size_t index) const
{
	return vars_[index].fval;
}


inline double MyVarList::DoubleVal(size_t index) const
{
	return vars_[index].dval;
}


inline std::string MyVarList::StringVal(size_t index) const
{
	return vars_[index].str;
}


inline size_t MyVarList::GetCount() const
{
	return vars_.size();
}

#endif // __my_var_list_h__
