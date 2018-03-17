#include "DataBase.h"

#include <iostream>
#include <iomanip>
#include <set>

#include "xxt.h"

//[portotype]
int callback(void* para, int nCount, char** pValue, char** pName);//传给sqlite3_exec的回调函数

DataBase::DataBase()
{
	nRes = -1;
	showLog = true;
}


DataBase::~DataBase()
{
	delete selectResult;
}

void DataBase::setPath(std::string path)
{
	this->path = path;
}

std::string DataBase::getPath(){ return path; }

void DataBase::setShowLog(bool showLog)
{
	this->showLog = showLog;
}

void DataBase::open(bool inMemory)//[]可能写个openInMemory,将文件数据库拷贝到内存中进行操作
{
	if (inMemory) path = ":memory:";//[]内存数据库使用":memory:"作为文件名
	nRes = sqlite3_open(path.c_str(), &ptDB);
	if (nRes != SQLITE_OK)
	{
		if (showLog) std::cerr << path << "数据库打开失败" << sqlite3_errmsg(ptDB) << "即将关闭数据库！" << std::endl;
		close();
	}
	else
	{
		if (showLog) std::cout << path << "数据库打开成功！" << std::endl;
	}
}

void DataBase::copyTo(DataBase& targetDB)
{
	if (this->ptDB == targetDB.ptDB)
	{
		std::cerr << "输入、输出数据库不能相同！" << std::endl;
		return;
	}
//	if (this->ptDB == NULL) std::cerr << "输入数据库未打开！" << std::endl;
//	if (targetDB.ptDB == NULL) std::cerr << "输出数据库未打开！" << std::endl;
	sqlite3_backup* ptDB_Backup = sqlite3_backup_init(targetDB.ptDB, "main", this->ptDB, "main");//用于进行备份
	if (ptDB_Backup){
		(void)sqlite3_backup_step(ptDB_Backup, -1);
		(void)sqlite3_backup_finish(ptDB_Backup);
	}
	int nRes = sqlite3_errcode(targetDB.ptDB);
	std::string commandName = "将数据库" + this->path + "拷贝到数据库" + targetDB.path + "中";
	if (nRes != SQLITE_OK)
	{
		if (showLog) std::cout << "[×] " << commandName << "失败：" << std::endl;
	}
	else
	{
		if (showLog) std::cout << "[√] " << commandName << "成功：" << std::endl;
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
	strSql = "CREATE TABLE " + sTableName + " (";
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

	message = "创建表格" + sTableName;
	return executeCommand(message, ptDB, strSql);
}

bool DataBase::deleteTable(const std::string& sTableName)
{
	/*
	DROP TABLE database_name.table_name;
	*/
	//删除表格
	strSql = "DROP TABLE " + sTableName + ";";

	message = "删除表格" + sTableName;
	return executeCommand(message, ptDB, strSql);
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
	strSql = "INSERT INTO " + sTableName + " ";
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

	message = "在" + sTableName + "中添加数据" + content;
	return executeCommand(message, ptDB, strSql);
}

bool DataBase::deleteRow(const std::string& sTableName, const std::string& sCondition)
{
	/*
	DELETE FROM table_name
	WHERE [condition];
	*/
	//准备删除语句
	strSql = "DELETE FROM " + sTableName;
	//条件[可选]
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
	return executeCommand(message, ptDB, strSql);
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
	strSql = "UPDATE " + sTableName + " SET ";
	//写入各列修改内容
	for each(auto curColumnNameValuePair in columnNameValuePairs)
	{
		strSql = strSql + curColumnNameValuePair.first + " = " + curColumnNameValuePair.second + ", ";
		columnUpdateContent = columnUpdateContent + curColumnNameValuePair.first + " = " + curColumnNameValuePair.second + " | ";
	}
	strSql = strSql.substr(0, strSql.size() - 2);
	columnUpdateContent = columnUpdateContent.substr(0, columnUpdateContent.size() - 2);
	//条件[可选]
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
	return executeCommand(message, ptDB, strSql);
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
	strSql = "SELECT ";
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
	if (sCondition != NO_VALUE)
	{
		strSql = strSql + " WHERE " + sCondition + ";";
		message = "从" + sTableName + "中选择满足" + sCondition + "条件的列数据:\n" + std::string(5, ' ') + columnSelectContent;
	}
	else
	{
		strSql += ";";
		message = "从" + sTableName + "中选择下列所有列数据:\n" + std::string(5, ' ') + columnSelectContent;
	}
	selectResult->clear();
	bool result = executeCommand(message, ptDB, strSql, callback, selectResult);
	return result;
}

bool DataBase::executeCommand
(std::string commandName
, sqlite3* ptDB
, std::string sqlCommand
, int(*callback)(void* selectResult, int, char**, char**)
, void* call_Back_1th_parameter)
{
	nRes = sqlite3_exec(ptDB, sqlCommand.c_str(), callback, call_Back_1th_parameter, &cErrMsg);//执行状态
	if (nRes != SQLITE_OK)
	{
		if (showLog) std::cout << "[×] " << commandName << "失败：" << cErrMsg << std::endl << std::endl;
		return false;
	}
	else
	{
		if (showLog) std::cout << "[√] " << commandName << "成功：" << std::endl;
		return true;
	}
}

void DataBase::printSelectResult()
{
	std::cout << "一共查询出" << selectResult->count << "条记录:" << std::endl;
	//输出表头
	std::cout << std::setw(COLUMN_WIDTH_INDEX) << "序号";
	for each(auto curColumnName in selectResult->columnNames)
	{
		std::cout << std::setw(COLUMN_WIDTH) << curColumnName << "|";
	}
	std::cout << std::endl;
	//输出内容
	for (int i = 0; i < selectResult->count; i++)
	{
		std::cout << std::setw(COLUMN_WIDTH_INDEX) << i + 1;
		for each(auto curColumnValue in selectResult->columnValues[i])
		{
			std::cout << std::setw(COLUMN_WIDTH) << curColumnValue << "|";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

void DataBase::printTable(std::string sTableName)
{
	selectColumn(sTableName);
	printSelectResult();
}

int callback(void* para, int nCount, char** pValue, char** pName) {
	/*****************************************************************************
	sqlite3 每查到一条记录，就调用一次这个回调！
	para是你在 sqlite3_exec 里传入的 void * 参数, 通过para参数，你可以传入一些特殊的指针（比如类指  针、结构指针），然后这里强制转换成对应的类型（这里面是void*类型，必须强制转换成你的类型才可用），进行数据写入
	nCount是这一条(行)记录有多少个字段 (即这条记录有多少列)
	char ** pValue 是个关键值，查出来的数据都保存在这里，它实际上是个1维数组（不要以为是2维数组），每一个元素都是一个 char* 值，是一个字段内容（用字符串来表示，以/0结尾）
	char ** pName 跟pValue是对应的，表示这个字段的字段名称, 也是个1维数组
	*****************************************************************************/

	DataBase::SelectResult* curResult = (DataBase::SelectResult*) para;
	//写入表头
	if (curResult->columnNames.size() == 0)
	{//表头只用写入一次
		for (int i = 0; i < nCount; i++)
		{
			curResult->columnNames.insert(curResult->columnNames.end(), pName[i]);
		}
	}
	//写入数据
	std::vector<std::string> curColumnValues;
	for (int i = 0; i < nCount; i++)
	{
		std::string curColumnValue = pValue[i] == NULL ? "" : pValue[i];//对录入信息不完整的情况进行过滤
		curColumnValues.insert(curColumnValues.end(), curColumnValue);
	}
	curResult->columnValues.insert(curResult->columnValues.end(), curColumnValues);
	curResult->count++;//每查到一条结果就运行一次，因此每次进来都表明查出的结果数量要++
	/*
	//输出每次查询出的结果
	std::string result;
	for (int i = 0; i < nCount; i++)
	{
	result = result + pName[i] + " = " + pValue[i] + " | ";
	}
	std::cout << result << std::endl;
	*/
	return 0;
}