#include <iostream>
#include <assert.h>
#include "sqlite3.h"
#include "microprofile/microprofile.h"

static int select_callback(void*, int column_num, char** column_values, char** column_names)
{
	for (int i = 0; i < column_num; ++i)
	{
		std::cout << column_values[i] << " ";
	}
	std::cout << std::endl;
	return 0;
}

static void hello_world()
{
	//打开/创建数据库
	sqlite3* db = nullptr;
	int errorno = sqlite3_open("test.db", &db);
	assert(errorno == SQLITE_OK);
	char* errormsg = nullptr;

	//删除表
	errorno = sqlite3_exec(db, "drop table chat_rec_1000", nullptr, nullptr, nullptr);
	//创建表
	errorno = sqlite3_exec(db, "create table chat_rec_1000(id integer primary key autoincrement,sender_uid varchar(100),sender_name varchar(100),message_body varchar(100),message_meta varchar(100))", nullptr, nullptr, nullptr);
	//插入数据
	errorno = sqlite3_exec(db, "insert into chat_rec_1000(sender_uid,sender_name,message_body,message_meta) values('e45678','tom','how old are you', '')", nullptr, nullptr, nullptr);
	errorno = sqlite3_exec(db, "insert into chat_rec_1000(sender_uid,sender_name,message_body,message_meta) values('9876ac','jerry','i am 3 years old', '')", nullptr, nullptr, nullptr);
	//查询表的所有行
	errorno = sqlite3_exec(db, "select * from chat_rec_1000", select_callback, nullptr, nullptr);
	//删除表中所有行
	errorno = sqlite3_exec(db, "delete from chat_rec_1000", select_callback, nullptr, nullptr);

	//关闭数据库
	sqlite3_close(db);
}

static void test_auction()
{
#ifdef _DEBUG 
	int TEST_INSERT_COUNT = 100;
	int TEST_SELECT_COUNT = 100;
#else
	int TEST_INSERT_COUNT = 100000;
	int TEST_SELECT_COUNT = 100000;
#endif	

	//创建内存数据库
	sqlite3* db = nullptr;
	int errorno = sqlite3_open(":memory:", &db);
	assert(errorno == SQLITE_OK);

	//创建物品表
	errorno = sqlite3_exec(db, "CREATE TABLE t_products(c_id INTEGER PRIMARY KEY AUTOINCREMENT,\
								c_min_price INTEGER,\
								c_max_price INTEGER,\
								c_cur_price INTEGER,\
								c_color_level INTEGER,\
								c_require_level INTEGER,\
								c_item_data VARCHAR(100))", nullptr, nullptr, nullptr);
	assert(errorno == SQLITE_OK);

	//创建索引
	errorno = sqlite3_exec(db, "CREATE INDEX index_auction ON t_products(c_min_price,c_max_price,c_cur_price,c_color_level,c_require_level)", nullptr, nullptr, nullptr);
	assert(errorno == SQLITE_OK);

	//插入数据
	{
		for (int i = 0; i < TEST_INSERT_COUNT; ++i)
		{
			int min_price = rand() % 100;
			int max_price = min_price + 1 + rand() % 1000;
			int cur_price = min_price + rand() % (max_price - min_price);
			int color_level = rand() % 5;
			int require_level = rand() % 30;
			char sql[512];
			sprintf_s(sql, "INSERT INTO t_products(c_min_price,c_max_price,c_cur_price,c_color_level,c_require_level) VALUES(%d,%d,%d,%d,%d)", min_price, max_price, cur_price, color_level, require_level);
		
			MICROPROFILE_SCOPEI("auction", "insert", MP_YELLOW);
			errorno = sqlite3_exec(db, sql, nullptr, nullptr, nullptr);
			assert(errorno == SQLITE_OK);
		}
	}

	//查询数据
	{
		auto select_callback = [](void* ud, int column_num, char** column_values, char** column_names) {
			int* count = (int*)ud;
			*count = atoi(column_values[0]);
			return 0;
		};

		for (int i = 0; i < TEST_SELECT_COUNT; ++i)
		{
			char sql[512];
			sprintf_s(sql, "SELECT COUNT(*) FROM t_products");

			MICROPROFILE_SCOPEI("auction", "SELECT COUNT(*) FROM t_products", MP_SNOW);
			int count = 0;
			errorno = sqlite3_exec(db, sql, select_callback, &count, nullptr);
			assert(errorno == SQLITE_OK);
		}
	}

	//查询数据
	{
		auto select_callback = [](void* ud, int column_num, char** column_values, char** column_names) {
			int* count = (int*)ud;
			*count = atoi(column_values[0]);
			return 0;
		};

		for (int i = 0; i < TEST_SELECT_COUNT; ++i)
		{
			char sql[512];
			sprintf_s(sql, "SELECT COUNT(*) FROM t_products WHERE c_color_level = 3");

			MICROPROFILE_SCOPEI("auction", "SELECT COUNT(*) FROM t_products WHERE c_color_level = 3", MP_MISTYROSE);
			int count = 0;
			errorno = sqlite3_exec(db, sql, select_callback, &count, nullptr);
			assert(errorno == SQLITE_OK);
		}
	}

	//TODO:更新数据

	//TODO:删除数据

	//关闭数据库
	sqlite3_close(db);
}

int main()
{
	MicroProfileOnThreadCreate("Main");
	MicroProfileSetEnableAllGroups(true);
	MicroProfileSetForceMetaCounters(true);

	srand((unsigned int)time(nullptr));
	test_auction();

	MicroProfileDumpFileImmediately("profile.html", nullptr, nullptr);
	MicroProfileShutdown();

 	return 0;
}
