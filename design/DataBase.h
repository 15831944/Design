#pragma once

#include <string>
#include <vector>

#include "sqlite3.h"

class DataBase
{
public://通用数据
	enum E_SQL_Variable_Type
	{
		E_SQLVT_NULL,//值是一个 NULL 值
		E_SQLVT_INTEGER,//值是一个带符号的整数，根据值的大小存储在 1、2、3、4、6 或 8 字节中
		E_SQLVT_REAL,//值是一个浮点值，存储为 8 字节的 IEEE 浮点数字
		E_SQLVT_TEXT,//值是一个文本字符串，使用数据库编码（UTF-8、UTF-16BE 或 UTF-16LE）存储
		E_SQLVT_BLOB//值是一个 blob 数据，完全根据它的输入存储，例如直接存储图像、音频等
	};

public:
	DataBase();
	~DataBase();

	void setPath(std::string);//设置路径
	std::string getPath();//获取路径
	void open();//打开数据库
	int close();//关闭数据库
	bool createTable
		(const std::string& sTableName
		, std::vector<std::string> columnNameAndTypes);//创建表格
	bool addUser(const std::string& sName, const std::string& sAge);//添加行
	bool deleteUser(const std::string& sName);//删除行
	bool modifyUser(const std::string& sName, const std::string& sAge);//修改行

private:
	std::string path;//[]数据库文件路径，采用utf-8编码，如果路径中包含中文则需要进行编码转换
	sqlite3* ptDB;//连接上的数据库
	int stage;//数据库状态
	char* cErrMsg;//错误信息

	bool executeCommand//执行SQL命令
		(std::string commandName
		, sqlite3* ptDB//数据库实例(指针)
		, std::string sqlCommand//SQL命令
		, int(*callback)(void*, int, char**, char**)//回调函数//[?]这是什么用法？
		, void *);//做为回调函数的第一个参数//[?]函数声明不用写变量名？

	static int callback//[?]不知道干什么用的，好像是给sqlite3_exec的第3个参数用的
		(void *NotUsed//由 sqlite3_exec() 的第四个参数传递而来
		, int argc//表的列数
		, char **argv//指向查询结果的指针数组, 可以由 sqlite3_column_text() 得到
		, char **azColName);//指向表头名的指针数组, 可以由 sqlite3_column_name() 得到

};

