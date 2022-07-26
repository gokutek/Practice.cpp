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

static void hello_world()
{
	//��/�������ݿ�
	sqlite3* db = nullptr;
	int errorno = sqlite3_open("test.db", &db);
	assert(errorno == SQLITE_OK);
	char* errormsg = nullptr;

	//ɾ����
	errorno = sqlite3_exec(db, "drop table chat_rec_1000", nullptr, nullptr, nullptr);
	//������
	errorno = sqlite3_exec(db, "create table chat_rec_1000(id integer primary key autoincrement,sender_uid varchar(100),sender_name varchar(100),message_body varchar(100),message_meta varchar(100))", nullptr, nullptr, nullptr);
	//��������
	errorno = sqlite3_exec(db, "insert into chat_rec_1000(sender_uid,sender_name,message_body,message_meta) values('e45678','tom','how old are you', '')", nullptr, nullptr, nullptr);
	errorno = sqlite3_exec(db, "insert into chat_rec_1000(sender_uid,sender_name,message_body,message_meta) values('9876ac','jerry','i am 3 years old', '')", nullptr, nullptr, nullptr);
	//��ѯ���������
	errorno = sqlite3_exec(db, "select * from chat_rec_1000", select_callback, nullptr, nullptr);
	//ɾ������������
	errorno = sqlite3_exec(db, "delete from chat_rec_1000", select_callback, nullptr, nullptr);

	//�ر����ݿ�
	sqlite3_close(db);
}

int main()
{
 	return 0;
}
