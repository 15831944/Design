#include "DataBase.h"

#include <iostream>
#include <set>

DataBase::DataBase()
{
	stage = -1;
}


DataBase::~DataBase(){}

void DataBase::setPath(std::string path)
{
	this->path = path;
}

std::string DataBase::getPath(){ return path; }

void DataBase::open()
{
	stage = sqlite3_open(path.c_str(), &ptDB);
	if (stage != SQLITE_OK)
	{
		std::cerr << path << "数据库打开失败，即将关闭数据库！" << std::endl;
		close();
	}
	else
	{
		std::cerr << path << "数据库打开成功！" << std::endl;
	}
}

int DataBase::close()
{
	return sqlite3_close(ptDB);
}

bool DataBase::createTable
(const std::string& sTableName
, std::vector<std::string> columnNameAndTypes
, std::vector<std::string> primaryKeys)
{
/*
CREATE TABLE table_name(
column0 datatype PRIMARY KEY(one or more columns),//主键column前加PRIMARY KEY,可以没有或者有多个主键
column1 datatype,
column2 datatype,
.....
columnN datatype,
);
*/

	std::set<std::string> visitedStr;//主键用过的Column
	//创建表头
	std::string strSql = "CREATE TABLE " + sTableName + " (";
	//创建主键Column[可选]
	if (primaryKeys[0] != NO_VALUE)
	{
		for each(std::string curPrimaryKey in primaryKeys)
		{
			if (curPrimaryKey != ""){
				strSql = strSql + curPrimaryKey + " PRIMARY KEY,";
				visitedStr.insert(visitedStr.end(), curPrimaryKey);
			}
		}
	}
	//创建普通Column,主键用过的column不再创建
	for each(std::string curColumnNameAndType in columnNameAndTypes)
	{
		if (visitedStr.count(curColumnNameAndType) == 0) strSql = strSql + curColumnNameAndType + ",";
	}
	strSql = strSql.substr(0, strSql.size() - 1);
	strSql += ");";

	std::string message = "创建表格" + sTableName;
	return executeCommand(message, ptDB, strSql, 0, 0);
}

bool DataBase::deleteTable(const std::string& sTableName)
{
/*
DROP TABLE database_name.table_name;
*/
	std::string strSql = "DROP TABLE " + sTableName + ";";

	std::string message = "删除表格" + sTableName;
	return executeCommand(message, ptDB, strSql, 0, 0);
}

bool DataBase::addRow
(const std::string& sTableName
, const std::vector<std::string>& columnValues
, const std::vector<std::string>& columnNames)
{
/*
INSERT INTO tableName [(column1, column2, column3,...columnN)] VALUES (value1, value2, value3,...valueN);//[(column1, column2, column3,...columnN)]选填
*/
	std::string strSql = "INSERT INTO " + sTableName + " ";
	//创建column写入位置[可选]
	if (columnNames[0] != NO_VALUE)
	{
		strSql += "( ";
		for each(std::string curColumnName in columnNames)
		{
			if (curColumnName != ""){
				strSql += curColumnName + " ,";
			}
		}
		strSql = strSql.substr(0, strSql.size() - 1);
		strSql += ") ";
	}
	//创建columnValues
	strSql += "VALUES (";
	std::string content;//创建内容
	for each(std::string curColumnValue in columnValues)
	{
		strSql += curColumnValue + " ,";
		content += curColumnValue + "|";
	}
	content = content.substr(0, content.size() - 1);
	strSql = strSql.substr(0, strSql.size() - 1);
	strSql += ");";

	std::string message = "在" + sTableName + "中添加数据" + content;
	return executeCommand(message, ptDB, strSql, 0, 0);
}

bool DataBase::deleteRow(const std::string& sTableName, const std::string& sCondition)
{
/*
DELETE FROM table_name
WHERE [condition];
*/
	std::string strSql = "DELETE FROM " + sTableName;
	std::string message;
	if (sCondition != NO_VALUE)
	{
		strSql = strSql + " WHERE " + sCondition + ";";
		message = "从" + sTableName + "中删除满足" + sCondition + "的数据";
	}
	else
	{
		strSql += "';";
		message = "从" + sTableName + "中删除所有数据";
	}
	return executeCommand(message, ptDB, strSql, 0, 0);
}

bool DataBase::setRow(const std::string& sName, const std::string& sAge)
{
	std::string strSql = "";
	strSql += "update user set age =";
	strSql += sAge;
	strSql += " where name='";
	strSql += sName;
	strSql += "';";

	return executeCommand("修改数据", ptDB, strSql, 0, 0);
}

bool DataBase::executeCommand
(std::string commandName
, sqlite3* ptDB
, std::string sqlCommand
, int(*callback)(void*, int, char**, char**)
, void *)
{
	char* cErrMsg;//错误信息
	int nRes = sqlite3_exec(ptDB, sqlCommand.c_str(), 0, 0, &cErrMsg);//执行状态
	if (nRes != SQLITE_OK)
	{
		std::cout << "[×] " << commandName << "失败：" << cErrMsg << std::endl;
		return false;
	}
	else
	{
		std::cout << "[√] " << commandName << "成功：" << std::endl;
		return true;
	}
}

/*
static int DataBase::callback(void *NotUsed, int argc, char **argv, char **azColName)
{
	return 0;
}*/