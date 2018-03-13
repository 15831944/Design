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
		std::cerr << path << "���ݿ��ʧ�ܣ������ر����ݿ⣡" << std::endl;
		close();
	}
	else
	{
		std::cerr << path << "���ݿ�򿪳ɹ���" << std::endl;
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
column0 datatype PRIMARY KEY(one or more columns),//����columnǰ��PRIMARY KEY,����û�л����ж������
column1 datatype,
column2 datatype,
.....
columnN datatype,
);
*/

	std::set<std::string> visitedStr;//�����ù���Column
	//������ͷ
	std::string strSql = "CREATE TABLE " + sTableName + " (";
	//��������Column[��ѡ]
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
	//������ͨColumn,�����ù���column���ٴ���
	for each(std::string curColumnNameAndType in columnNameAndTypes)
	{
		if (visitedStr.count(curColumnNameAndType) == 0) strSql = strSql + curColumnNameAndType + ",";
	}
	strSql = strSql.substr(0, strSql.size() - 1);
	strSql += ");";

	std::string message = "�������" + sTableName;
	return executeCommand(message, ptDB, strSql, 0, 0);
}

bool DataBase::deleteTable(const std::string& sTableName)
{
/*
DROP TABLE database_name.table_name;
*/
	std::string strSql = "DROP TABLE " + sTableName + ";";

	std::string message = "ɾ�����" + sTableName;
	return executeCommand(message, ptDB, strSql, 0, 0);
}

bool DataBase::addRow
(const std::string& sTableName
, const std::vector<std::string>& columnValues
, const std::vector<std::string>& columnNames)
{
/*
INSERT INTO tableName [(column1, column2, column3,...columnN)] VALUES (value1, value2, value3,...valueN);//[(column1, column2, column3,...columnN)]ѡ��
*/
	std::string strSql = "INSERT INTO " + sTableName + " ";
	//����columnд��λ��[��ѡ]
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
	//����columnValues
	strSql += "VALUES (";
	std::string content;//��������
	for each(std::string curColumnValue in columnValues)
	{
		strSql += curColumnValue + " ,";
		content += curColumnValue + "|";
	}
	content = content.substr(0, content.size() - 1);
	strSql = strSql.substr(0, strSql.size() - 1);
	strSql += ");";

	std::string message = "��" + sTableName + "���������" + content;
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
		message = "��" + sTableName + "��ɾ������" + sCondition + "������";
	}
	else
	{
		strSql += "';";
		message = "��" + sTableName + "��ɾ����������";
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

	return executeCommand("�޸�����", ptDB, strSql, 0, 0);
}

bool DataBase::executeCommand
(std::string commandName
, sqlite3* ptDB
, std::string sqlCommand
, int(*callback)(void*, int, char**, char**)
, void *)
{
	char* cErrMsg;//������Ϣ
	int nRes = sqlite3_exec(ptDB, sqlCommand.c_str(), 0, 0, &cErrMsg);//ִ��״̬
	if (nRes != SQLITE_OK)
	{
		std::cout << "[��] " << commandName << "ʧ�ܣ�" << cErrMsg << std::endl;
		return false;
	}
	else
	{
		std::cout << "[��] " << commandName << "�ɹ���" << std::endl;
		return true;
	}
}

/*
static int DataBase::callback(void *NotUsed, int argc, char **argv, char **azColName)
{
	return 0;
}*/