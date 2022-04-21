#include <iostream>
#include <string>
#include "catch.hpp"
#include "rapidxml.hpp"
#include "rapidxml_iterators.hpp"
#include "rapidxml_print.hpp"
#include "rapidxml_utils.hpp"


/*
===============================================================================
第三方库rapidxml的使用：
1. allocate_attribute中会直接保存指针，如果传入的是一个字符串常量不会有问题。但如果
   类似传入的是一个全局字符串指针，并且还经常修改，那么就会有问题。最终生成的XML的所
   有属性值将会一个样；
===============================================================================
*/

namespace
{
	struct Student
	{
		std::string	name;
		std::string	age;
	};
}


static std::string CreateXMLDoc()
{
	rapidxml::xml_document<> doc;

	rapidxml::xml_node<>* decl_node = doc.allocate_node(rapidxml::node_pi, doc.allocate_string("xml version='1.0' encoding='utf-8'"));
	doc.append_node(decl_node);

	rapidxml::xml_node<> *root_node = doc.allocate_node(rapidxml::node_element, "class");
	doc.append_node(root_node);

	std::vector<Student> students = {
		{ "Tom", "18" },
		{ "Jerry", "20" },
	};

	for (auto &s : students)
	{
		rapidxml::xml_node<> *student_node = doc.allocate_node(rapidxml::node_element, "student");
		student_node->append_attribute(doc.allocate_attribute("name", s.name.c_str()));
		student_node->append_attribute(doc.allocate_attribute("age", s.age.c_str()));
		root_node->append_node(student_node);
	}

	std::string str;
	rapidxml::print(std::back_inserter(str), doc, 0);

	return str;
}


TEST_CASE("test write xml", "[rapidxml]")
{
	std::string const result =
		"<?xml version='1.0' encoding='utf-8' ?>\n"
		"<class>\n"
		"\t<student name=\"Tom\" age=\"18\"/>\n"
		"\t<student name=\"Jerry\" age=\"20\"/>\n"
		"</class>\n\n";

	std::string const doc = CreateXMLDoc();

	REQUIRE(result == doc);
}


TEST_CASE("test read xml", "[rapidxml]")
{
	std::string const text = CreateXMLDoc();

	rapidxml::xml_document<> doc;
	doc.parse<0>((char*)text.c_str());

	rapidxml::xml_node<> *root_node = doc.first_node();
	REQUIRE(!strcmp(root_node->name(), "class"));

	rapidxml::xml_node<> *tom_node = root_node->first_node();
	rapidxml::xml_node<> *jerry_node = tom_node->next_sibling();
	REQUIRE(jerry_node->next_sibling() == NULL);

	REQUIRE(!strcmp(tom_node->first_attribute("name")->value(), "Tom"));
	REQUIRE(!strcmp(tom_node->first_attribute("age")->value(), "18"));

	REQUIRE(!strcmp(jerry_node->first_attribute("name")->value(), "Jerry"));
	REQUIRE(!strcmp(jerry_node->first_attribute("age")->value(), "20"));
}
