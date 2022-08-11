#include <assert.h>
#include <functional>
#include <string>
#include <sstream>
#include <stdint.h>
#include <iostream>
#include <unordered_map>
#include <regex>
#include "json/json.h"

static std::string get_backend_type_name(std::string const& type)
{
	std::unordered_map<std::string, std::string> m;
	m["boolean"] = "bool";
	m["int64"] = "int64_t";
	m["string"] = "std::string";

	auto iter = m.find(type);
	if (iter != m.end()) { return iter->second; } 

	if (type.length() > 2 && type.substr(type.length() - 2) == "[]")
	{
		return std::string("std::vector<")  + type.substr(0, type.length()-2) + ">";
	}

	return type;
}

static std::string get_init_def(std::string const& backend_type)
{
	std::unordered_map<std::string, std::string> m;
	m["bool"] = " = false";
	m["int"] = " = 0";
	m["int64"] = " = 0";

	auto iter = m.find(backend_type);
	return iter != m.end() ? iter->second : "";
}

static std::string get_ident_str(size_t level)
{
	constexpr int PER_IDENT_SPACE_COUNT = 4;
	std::string str;
	str.resize(level * PER_IDENT_SPACE_COUNT, ' ');
	str.push_back(0);
	return str;
}

static int gen_serialize_field(std::ostringstream& oss, int ident_level, std::string const& backend_type, std::string const& field)
{
	if (backend_type == "std::string")
	{
		oss << get_ident_str(ident_level) << "ostream.AddString(" + field + ".c_str());" << std::endl;
	}
	else if (backend_type == "int")
	{
		oss << get_ident_str(ident_level) << "ostream.AddInt(" + field + ");" << std::endl;
	}
	else if (backend_type == "int64_t")
	{
		oss << get_ident_str(ident_level) << "ostream.AddInt64(" + field + ");" << std::endl;
	}
	else if (backend_type == "float")
	{
		oss << get_ident_str(ident_level) << "ostream.AddFloat(" + field + ");" << std::endl;
	}
	else if (std::string::npos != backend_type.find("std::vector"))
	{
		oss << get_ident_str(ident_level) <<  "ostream.AddInt((int)" + field + ".size());" << std::endl;
		oss << get_ident_str(ident_level) << "for (size_t i = 0; i < " + field + ".size(); ++i)" << std::endl;
		oss << get_ident_str(ident_level) << "{" << std::endl;
		oss << get_ident_str(ident_level+1) << field << "[i].Serialize(ostream);" << std::endl;
		oss << get_ident_str(ident_level) << "}" << std::endl;
	}
	else
	{
		return -1;
	}

	return 0;
}

static int gen_deserialize_field(std::ostringstream& oss, int ident_level, std::string const& backend_type, std::string const& field)
{
	if (backend_type == "std::string")
	{
		oss << get_ident_str(ident_level) << field << " = istream.GetString(offset++);" << std::endl;
	}
	else if (backend_type == "int")
	{
		oss << get_ident_str(ident_level) << field << " = istream.GetInt(offset++);" << std::endl;
	}
	else if (backend_type == "int64_t")
	{
		oss << get_ident_str(ident_level) << field << " = istream.GetInt64(offset++);" << std::endl;
	}
	else if (backend_type == "float")
	{
		oss << get_ident_str(ident_level) << field << " = istream.GetFloat(offset++);" << std::endl;
	}
	else if (std::string::npos != backend_type.find("std::vector"))
	{
#if 1
		char element_type[64] = { 0 };
		sscanf(backend_type.c_str(), "std::vector<%[^>]", element_type);
#else
		std::regex word_regex("std::vector<\\w*>");
		std::smatch match;
		std::regex_search(backend_type, match, word_regex, std::regex_constants::match_default);
		std::string element_type = match.str();
#endif

		oss << get_ident_str(ident_level) << "{" << std::endl;
		oss << get_ident_str(ident_level + 1) << "int count = istream.GetInt(offset++);" << std::endl;
		oss << get_ident_str(ident_level + 1) << "for (int i = 0; i < count; ++i)" << std::endl;
		oss << get_ident_str(ident_level + 1) << "{" << std::endl;
		oss << get_ident_str(ident_level + 2) << element_type << " data;" << std::endl;
		oss << get_ident_str(ident_level + 2) << "data.Deserialize(istream, offset);" << std::endl;
		oss << get_ident_str(ident_level + 2) << "field.push_back(data);" << std::endl;
		oss << get_ident_str(ident_level + 1) << "}" << std::endl;
		oss << get_ident_str(ident_level) << "}" << std::endl;
	}
	else
	{
		return -1;
	}

	return 0;
}

static int gen_code(std::ostringstream& oss, int ident_level, Json::Value const& node)
{
	Json::Value::Members fields = node.getMemberNames();

	//嵌套的结构体
	for (auto& field : fields)
	{
		if (!node[field].isString())
		{
			oss << get_ident_str(ident_level + 1) << "struct " << field << " {" << std::endl;
			gen_code(oss, ident_level + 1, node[field]);
			oss << get_ident_str(ident_level + 1) << "};" << std::endl;
			oss << std::endl;
		}
	}

	auto for_each_member = [&](std::function<void(std::string,std::string)> const& iter) {
		for (auto& field : fields)
		{
			if (node[field].isString())
			{
				std::string backend_type = get_backend_type_name(node[field].asString());
				iter(backend_type, field);
			}
		}
	};

	//声明成员变量
	for_each_member([&](std::string const& backend_type, std::string const& field) {
		oss << get_ident_str(ident_level + 1) << backend_type << " " << field << get_init_def(backend_type) << ";" << std::endl;
	});

	//序列化成员函数
	oss << std::endl;
	oss << get_ident_str(ident_level + 1) << "IMutList& Serialize(IMutList& ostream) const" << std::endl;
	oss << get_ident_str(ident_level + 1) << "{" << std::endl;
	for_each_member([&](std::string const& backend_type, std::string const& field) {
		gen_serialize_field(oss, ident_level+2, backend_type, field);
	});
	oss << get_ident_str(ident_level + 2) << "return ostream;" << std::endl;
	oss << get_ident_str(ident_level + 1) << "}" << std::endl;

	//反序列化成员函数
	oss << std::endl;
	oss << get_ident_str(ident_level + 1) << "void Deserialize(const IMutList& istream, size_t offset)" << std::endl;
	oss << get_ident_str(ident_level + 1) << "{" << std::endl;
	for_each_member([&](std::string const& backend_type, std::string const& field) {
		gen_deserialize_field(oss, ident_level + 2, backend_type, field);
	});
	oss << get_ident_str(ident_level + 1) << "}" << std::endl;

	return 0;
}

int main(int argc, char** argv)
{
	if (argc != 2)
	{
		std::cerr << "file name is not spec" << std::endl;
		return -1;
	}

	FILE* fp = fopen(argv[1], "r");
	if (!fp) 
	{
		std::cerr << "file not exist " << argv[1] << std::endl;
		return -1;
	}

	//读取JSON文件
	fseek(fp, 0, SEEK_END);
	long size = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	std::string document;
	document.resize(size+1);
	fread((void*)document.c_str(), 1, size, fp);
	fclose(fp);

	//解析JSON文件
	Json::Value root;
	Json::Reader reader;
	bool ok = reader.parse(document, root);
	assert(ok);

	//代码生成
	Json::Value::Members members = root.getMemberNames();
	for (auto& member : members)
	{
		std::ostringstream oss;
		oss << "struct " << member << " {" << std::endl;
		gen_code(oss, 0, root[member]);
		oss << "};" << std::endl;

		std::cout << oss.str() << std::endl;
	}

	return 0;
}
