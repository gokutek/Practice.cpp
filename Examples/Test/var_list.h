#ifndef VAR_LIST_H
#define VAR_LIST_H

#include <assert.h>
#include <variant>
#include <vector>
#include <string>


enum class VT
{
	VT_BOOL,
	VT_INT,
	VT_INT64,
	VT_FLOAT,
	VT_DOUBLE,
	VT_STRING,
};


class VarList
{
	struct Var 
	{
		VT type;
		std::variant<bool, int, int64_t, float, double, std::string> var;
	};

public:
	size_t GetCount() const;
	VT GetType(size_t i) const;
	VarList& AddBool(bool var);
	VarList& AddInt(int var);
	VarList& AddInt64(int64_t var);
	VarList& AddFloat(float var);
	VarList& AddDouble(double var);
	VarList& AddString(char const *var);

	bool BoolVar(size_t i) const;
	int IntVar(size_t i) const;
	int64_t Int64Var(size_t i) const;
	float FloatVar(size_t i) const;
	double DoubleVar(size_t i) const;
	char const * StringVar(size_t i) const;

private:
	std::vector<Var>	vars_;
};


inline size_t VarList::GetCount() const
{
	return vars_.size();
}


inline VT VarList::GetType(size_t i) const
{
	return vars_[i].type;
}


inline VarList& VarList::AddBool(bool var)
{
	Var v{ VT::VT_BOOL, var};
	vars_.emplace_back(v);
	return *this;
}


inline VarList& VarList::AddInt(int var)
{
	Var v{ VT::VT_INT, var };
	vars_.emplace_back(v);
	return *this;
}


inline VarList& VarList::AddInt64(int64_t var)
{
	Var v{ VT::VT_INT64, var };
	vars_.emplace_back(v);
	return *this;
}


inline VarList& VarList::AddFloat(float var)
{
	Var v{ VT::VT_FLOAT, var };
	vars_.emplace_back(v);
	return *this;
}


inline VarList& VarList::AddDouble(double var)
{
	Var v{ VT::VT_DOUBLE, var };
	vars_.emplace_back(v);
	return *this;
}


inline VarList& VarList::AddString(char const *var)
{
	Var v{ VT::VT_STRING, std::string(var) };
	vars_.emplace_back(v);
	return *this;
}


inline bool VarList::BoolVar(size_t i) const
{
	assert(vars_[i].type == VT::VT_BOOL);
	return std::get<bool>(vars_[i].var);
}


inline int VarList::IntVar(size_t i) const
{
	assert(vars_[i].type == VT::VT_INT);
	return std::get<int>(vars_[i].var);
}


inline int64_t VarList::Int64Var(size_t i) const
{
	assert(vars_[i].type == VT::VT_INT64);
	return std::get<int64_t>(vars_[i].var);
}


inline float VarList::FloatVar(size_t i) const
{
	assert(vars_[i].type == VT::VT_FLOAT);
	return std::get<float>(vars_[i].var);
}


inline double VarList::DoubleVar(size_t i) const
{
	assert(vars_[i].type == VT::VT_DOUBLE);
	return std::get<double>(vars_[i].var);
}


inline char const * VarList::StringVar(size_t i) const
{
	assert(vars_[i].type == VT::VT_STRING);
	std::string const &str = std::get<std::string>(vars_[i].var);
	return str.c_str();
}


inline VarList& operator<<(VarList &varlist, bool var)
{
	return varlist.AddBool(var);
}


inline VarList& operator<<(VarList &varlist, int var)
{
	return varlist.AddInt(var);
}


inline VarList& operator<<(VarList &varlist, int64_t var)
{
	return varlist.AddInt64(var);
}


inline VarList& operator<<(VarList &varlist, float var)
{
	return varlist.AddFloat(var);
}


inline VarList& operator<<(VarList &varlist, double var)
{
	return varlist.AddDouble(var);
}


inline VarList& operator<<(VarList &varlist, char const *var)
{
	return varlist.AddString(var);
}

#endif // VAR_LIST_H
