#include "DataBase.h"

#include <iostream>

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
/*----------
CREATE TABLE table_name(
column1 datatype,
column2 datatype,
column3 datatype,
.....
columnN datatype,
PRIMARY KEY(one or more columns)
);----------*/

	std::string strSql = "";
	//������ͷ
	strSql = strSql + "CREATE TABLE " + sTableName + " (";
	//����������
	for each(std::string curColumnNameAndType in columnNameAndTypes)
	{
		strSql = strSql + curColumnNameAndType + ",";
	}
	//��������
	strSql += "PRIMARY KEY ";
	for each(std::string curPrimaryKey in primaryKeys)
	{
		strSql = strSql + curPrimaryKey + ",";
	}
	strSql = strSql.substr(0, strSql.size() - 1);
	strSql += ");";

	return executeCommand("�������", ptDB, strSql, 0, 0);
}

bool DataBase::addUser(const std::string& sName, const std::string& sAge)
{
	std::string strSql = "";
	strSql += "insert into user(name,age)";
	strSql += "values('";
	strSql += sName;
	strSql += "',";
	strSql += sAge;
	strSql += ");";

	return executeCommand("�������", ptDB, strSql, 0, 0);
}

bool DataBase::deleteUser(const std::string& sName)
{
	std::string strSql = "";
	strSql += "delete from user where name='";
	strSql += sName;
	strSql += "';";

	return executeCommand("ɾ������", ptDB, strSql, 0, 0);
}

bool DataBase::modifyUser(const std::string& sName, const std::string& sAge)
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
		std::cout << commandName << "ʧ�ܣ�" << cErrMsg << std::endl;
		return false;
	}
	else
	{
		std::cout << commandName << "�ɹ���" << cErrMsg << std::endl;
		return true;
	}
}

/*
static int DataBase::callback(void *NotUsed, int argc, char **argv, char **azColName)
{
	return 0;
}*/