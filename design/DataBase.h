#pragma once

#include <string>
#include <vector>
#include <map>

#include "sqlite3.h"

static const std::string NO_VALUE = "NULL";//标识数据为空//[]怎么写在类里？

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
	bool createTable//创建表格
		(const std::string& sTableName
		, std::vector<std::string> columnNameAndTypes//column内容
		, std::vector<std::string> primaryKeys = std::vector<std::string>{NO_VALUE});//主键内容
	bool deleteTable(const std::string& sTableName);//删除表格

	bool addRow//添加行
		(const std::string& sTableName
		, const std::vector<std::string>& columnValues//添加行的各列数据
		, const std::vector<std::string>& columnNames = std::vector<std::string>{NO_VALUE});//在columnNames处对位添加columnValues
	bool deleteRow//删除满足特定条件的行[不写则删除所有行]
		(const std::string& sTableName
		, const std::string& sCondition = NO_VALUE);//条件[可选]
	bool setRow//修改列数据，可以附加WHERE语句[不写则修改列的所有数据]
		(const std::string& sTableName
		, const std::map<std::string, std::string> columnNameValuePairs//要修改的列数据,columnName=columnValue
		, const std::string& sCondition = NO_VALUE);//条件[可选]
	bool selectColumn//选择列，可以附加WHERE语句[不写则选择出所有列]
		(const std::string& sTableName
		, const std::vector<std::string> columnNames = std::vector<std::string>{NO_VALUE}//要选择的列名称
		, const std::string& sCondition = NO_VALUE);//条件[可选]

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

/*	static int callback//[?]不知道干什么用的，好像是给sqlite3_exec的第3个参数用的
		(void *NotUsed//由 sqlite3_exec() 的第四个参数传递而来
		, int argc//表的列数
		, char **argv//指向查询结果的指针数组, 可以由 sqlite3_column_text() 得到
		, char **azColName);//指向表头名的指针数组, 可以由 sqlite3_column_name() 得到
*/
};

