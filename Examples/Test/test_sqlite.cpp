#include <iostream>
#include <assert.h>
#include "sqlite3.h"

static int select_callback(void*, int column_num, char** column_values, char** column_names)
{
	for (int i = 0; i < column_num; ++i)
	{
		std::cout << column_values[i] << " ";
	}
	std::cout << std::endl;
	return 0;
}

int main()
{
	//打开/创建数据库
	sqlite3* db = nullptr;
	int errorno = sqlite3_open("test.db", &db);
	assert(errorno == SQLITE_OK);
	char* errormsg = nullptr;

	//创建表
	errorno = sqlite3_exec(db, "create table chat_rec_1000(id integer primary key autoincrement,name varchar(100))", nullptr, nullptr, nullptr);
	//插入数据
	errorno = sqlite3_exec(db, "insert into chat_rec_1000(name) values(\"hello\")", nullptr, nullptr, &errormsg);
	errorno = sqlite3_exec(db, "insert into chat_rec_1000(name) values(\"world\")", nullptr, nullptr, nullptr);
	//查询表的所有行
	errorno = sqlite3_exec(db, "select * from chat_rec_1000", select_callback, nullptr, nullptr);
	//删除表中所有行
	errorno = sqlite3_exec(db, "delete from chat_rec_1000", select_callback, nullptr, nullptr);

	//关闭数据库
	sqlite3_close(db);

 	return 0;
}
