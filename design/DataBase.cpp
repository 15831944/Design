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
	//删除表格
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
	//准备插入语句
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
	//准备删除语句
	std::string strSql = "DELETE FROM " + sTableName;
	//条件[可选]
	std::string message;
	if (sCondition != NO_VALUE)
	{
		strSql = strSql + " WHERE " + sCondition + ";";
		message = "从" + sTableName + "中删除满足" + sCondition + "条件的行数据";
	}
	else
	{
		strSql += "';";
		message = "从" + sTableName + "中删除所有数据";
	}
	return executeCommand(message, ptDB, strSql, 0, 0);
}

bool DataBase::setRow
(const std::string& sTableName
, const std::map<std::string, std::string> columnNameValuePairs
, const std::string& sCondition)
{
	/*
	UPDATE table_name
	SET column1 = value1, column2 = value2...., columnN = valueN
	WHERE [condition];//可选
	*/
	std::string columnUpdateContent;//具体修改内容
	//准备修改语句
	std::string strSql = "UPDATE " + sTableName + " SET ";
	//写入各列修改内容
	for each(auto curColumnNameValuePair in columnNameValuePairs)
	{
		strSql = strSql + curColumnNameValuePair.first + " = " + curColumnNameValuePair.second + ", ";
		columnUpdateContent = columnUpdateContent + curColumnNameValuePair.first + " = " + curColumnNameValuePair.second + " | ";
	}
	strSql = strSql.substr(0, strSql.size() - 2);
	columnUpdateContent = columnUpdateContent.substr(0, columnUpdateContent.size() - 2);
	//条件[可选]
	std::string message;
	if (sCondition != NO_VALUE)
	{
		strSql = strSql + " WHERE " + sCondition + ";";
		message = "从" + sTableName + "中修改满足" + sCondition + "条件的列数据:\n" + std::string(5, ' ') + columnUpdateContent;
	}
	else
	{
		strSql += "';";
		message = "从" + sTableName + "中修改以下所有列数据:\n" + std::string(5, ' ') + columnUpdateContent;
	}
	return executeCommand(message, ptDB, strSql, 0, 0);
}

//[?]回传函数怎么用？
int callback(void*para, int nCount, char** pValue, char** pName) {
	/*****************************************************************************
	sqlite 每查到一条记录，就调用一次这个回调
	para是你在 sqlite3_exec 里传入的 void * 参数, 通过para参数，你可以传入一些特殊的指针（比如类指  针、结构指针），然后在这里面强制转换成对应的类型（这里面是void*类型，必须强制转换成你的类型才可用）。然后操作这些数据
	nCount是这一条(行)记录有多少个字段 (即这条记录有多少列)
	char ** pValue 是个关键值，查出来的数据都保存在这里，它实际上是个1维数组（不要以为是2维数组），每一个元素都是一个 char* 值，是一个字段内容（用字符串来表示，以/0结尾）
	char ** pName 跟pValue是对应的，表示这个字段的字段名称, 也是个1维数组
	*****************************************************************************/
	std::string result;
	for (int i = 0; i < nCount; i++)
	{
		result = result + pName[i] + " = " + pValue[i] + "\n";
	}
	std::cout << result << std::endl;
	return 0;
}

bool DataBase::selectColumn
(const std::string& sTableName
, const std::vector<std::string> columnNames
, const std::string& sCondition)
{
	/*
	SELECT * FROM table_name WHERE [condition];
	SELECT column1, column2, columnN FROM table_name WHERE [condition];
	*/
	std::string columnSelectContent;//具体修改内容
	//准备选择语句
	std::string strSql = "SELECT ";
	//写入选择列名称[可选]
	if (columnNames[0] != NO_VALUE)
	{
		for each(auto curColumnName in columnNames)
		{
			strSql = strSql + curColumnName + ", ";
			columnSelectContent += curColumnName + " | ";
		}
		strSql = strSql.substr(0, strSql.size() - 2);
		columnSelectContent = columnSelectContent.substr(0, columnSelectContent.size() - 2);
	}
	else
	{
		strSql += "*";
	}
	strSql += " FROM " + sTableName;
	//条件[可选]
	std::string message;
	if (sCondition != NO_VALUE)
	{
		strSql = strSql + " WHERE " + sCondition + ";";
		message = "从" + sTableName + "中选择满足" + sCondition + "条件的列数据:\n" + std::string(5, ' ') + columnSelectContent;
	}
	else
	{
		strSql += "';";
		message = "从" + sTableName + "中选择下列所有列数据:\n" + std::string(5, ' ') + columnSelectContent;
	}
	return executeCommand(message, ptDB, strSql, callback, 0);//[?]回传函数怎么用？
}

bool DataBase::executeCommand
(std::string commandName
, sqlite3* ptDB
, std::string sqlCommand
, int(*callback)(void*, int, char**, char**)
, void *)
{
	char* cErrMsg;//错误信息
	int nRes = sqlite3_exec(ptDB, sqlCommand.c_str(), callback, 0, &cErrMsg);//执行状态
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