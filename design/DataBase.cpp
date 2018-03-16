#include "DataBase.h"

#include <iostream>
#include <iomanip>
#include <set>

#include "xxt.h"

//[portotype]
int callback(void* para, int nCount, char** pValue, char** pName);//����sqlite3_exec�Ļص�����

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

void DataBase::open(bool inMemory)//[]����д��openInMemory,���ļ����ݿ⿽�����ڴ��н��в���
{
	if (inMemory) path = ":memory:";//[]�ڴ����ݿ�ʹ��":memory:"��Ϊ�ļ���
	nRes = sqlite3_open(path.c_str(), &ptDB);
	if (nRes != SQLITE_OK)
	{
		if (showLog) std::cerr << path << "���ݿ��ʧ��" << sqlite3_errmsg(ptDB) << "�����ر����ݿ⣡" << std::endl;
		close();
	}
	else
	{
		if (showLog) std::cout << path << "���ݿ�򿪳ɹ���" << std::endl;
	}
}

void DataBase::copyTo(DataBase& targetDB)
{
	if (this->ptDB == targetDB.ptDB)
	{
		std::cerr << "���롢������ݿⲻ����ͬ��" << std::endl;
		return;
	}
//	if (this->ptDB == NULL) std::cerr << "�������ݿ�δ�򿪣�" << std::endl;
//	if (targetDB.ptDB == NULL) std::cerr << "������ݿ�δ�򿪣�" << std::endl;
	sqlite3_backup* ptDB_Backup = sqlite3_backup_init(targetDB.ptDB, "main", this->ptDB, "main");//���ڽ��б���
	if (ptDB_Backup){
		(void)sqlite3_backup_step(ptDB_Backup, -1);
		(void)sqlite3_backup_finish(ptDB_Backup);
	}
	int nRes = sqlite3_errcode(targetDB.ptDB);
	std::string commandName = "�����ݿ�" + this->path + "���������ݿ�" + targetDB.path + "��";
	if (nRes != SQLITE_OK)
	{
		if (showLog) std::cout << "[��] " << commandName << "ʧ�ܣ�" << std::endl;
	}
	else
	{
		if (showLog) std::cout << "[��] " << commandName << "�ɹ���" << std::endl;
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
	strSql = "CREATE TABLE " + sTableName + " (";
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

	message = "�������" + sTableName;
	return executeCommand(message, ptDB, strSql);
}

bool DataBase::deleteTable(const std::string& sTableName)
{
	/*
	DROP TABLE database_name.table_name;
	*/
	//ɾ�����
	strSql = "DROP TABLE " + sTableName + ";";

	message = "ɾ�����" + sTableName;
	return executeCommand(message, ptDB, strSql);
}

bool DataBase::addRow
(const std::string& sTableName
, const std::vector<std::string>& columnValues
, const std::vector<std::string>& columnNames)
{
	/*
	INSERT INTO tableName [(column1, column2, column3,...columnN)] VALUES (value1, value2, value3,...valueN);//[(column1, column2, column3,...columnN)]ѡ��
	*/
	//׼���������
	strSql = "INSERT INTO " + sTableName + " ";
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

	message = "��" + sTableName + "���������" + content;
	return executeCommand(message, ptDB, strSql);
}

bool DataBase::deleteRow(const std::string& sTableName, const std::string& sCondition)
{
	/*
	DELETE FROM table_name
	WHERE [condition];
	*/
	//׼��ɾ�����
	strSql = "DELETE FROM " + sTableName;
	//����[��ѡ]
	if (sCondition != NO_VALUE)
	{
		strSql = strSql + " WHERE " + sCondition + ";";
		message = "��" + sTableName + "��ɾ������" + sCondition + "������������";
	}
	else
	{
		strSql += "';";
		message = "��" + sTableName + "��ɾ����������";
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
	WHERE [condition];//��ѡ
	*/
	std::string columnUpdateContent;//�����޸�����
	//׼���޸����
	strSql = "UPDATE " + sTableName + " SET ";
	//д������޸�����
	for each(auto curColumnNameValuePair in columnNameValuePairs)
	{
		strSql = strSql + curColumnNameValuePair.first + " = " + curColumnNameValuePair.second + ", ";
		columnUpdateContent = columnUpdateContent + curColumnNameValuePair.first + " = " + curColumnNameValuePair.second + " | ";
	}
	strSql = strSql.substr(0, strSql.size() - 2);
	columnUpdateContent = columnUpdateContent.substr(0, columnUpdateContent.size() - 2);
	//����[��ѡ]
	if (sCondition != NO_VALUE)
	{
		strSql = strSql + " WHERE " + sCondition + ";";
		message = "��" + sTableName + "���޸�����" + sCondition + "������������:\n" + std::string(5, ' ') + columnUpdateContent;
	}
	else
	{
		strSql += "';";
		message = "��" + sTableName + "���޸���������������:\n" + std::string(5, ' ') + columnUpdateContent;
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
	std::string columnSelectContent;//�����޸�����
	//׼��ѡ�����
	strSql = "SELECT ";
	//д��ѡ��������[��ѡ]
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
	//����[��ѡ]
	if (sCondition != NO_VALUE)
	{
		strSql = strSql + " WHERE " + sCondition + ";";
		message = "��" + sTableName + "��ѡ������" + sCondition + "������������:\n" + std::string(5, ' ') + columnSelectContent;
	}
	else
	{
		strSql += ";";
		message = "��" + sTableName + "��ѡ����������������:\n" + std::string(5, ' ') + columnSelectContent;
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
	nRes = sqlite3_exec(ptDB, sqlCommand.c_str(), callback, call_Back_1th_parameter, &cErrMsg);//ִ��״̬
	if (nRes != SQLITE_OK)
	{
		if (showLog) std::cout << "[��] " << commandName << "ʧ�ܣ�" << cErrMsg << std::endl << std::endl;
		return false;
	}
	else
	{
		if (showLog) std::cout << "[��] " << commandName << "�ɹ���" << std::endl;
		return true;
	}
}

void DataBase::printSelectResult()
{
	std::cout << "һ����ѯ��" << selectResult->count << "����¼:" << std::endl;
	//�����ͷ
	std::cout << std::setw(COLUMN_WIDTH_INDEX) << "���";
	for each(auto curColumnName in selectResult->columnNames)
	{
		std::cout << std::setw(COLUMN_WIDTH) << curColumnName << "|";
	}
	std::cout << std::endl;
	//�������
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
	sqlite3 ÿ�鵽һ����¼���͵���һ������ص���
	para������ sqlite3_exec �ﴫ��� void * ����, ͨ��para����������Դ���һЩ�����ָ�루������ָ  �롢�ṹָ�룩��Ȼ������ǿ��ת���ɶ�Ӧ�����ͣ���������void*���ͣ�����ǿ��ת����������Ͳſ��ã�����������д��
	nCount����һ��(��)��¼�ж��ٸ��ֶ� (��������¼�ж�����)
	char ** pValue �Ǹ��ؼ�ֵ������������ݶ������������ʵ�����Ǹ�1ά���飨��Ҫ��Ϊ��2ά���飩��ÿһ��Ԫ�ض���һ�� char* ֵ����һ���ֶ����ݣ����ַ�������ʾ����/0��β��
	char ** pName ��pValue�Ƕ�Ӧ�ģ���ʾ����ֶε��ֶ�����, Ҳ�Ǹ�1ά����
	*****************************************************************************/

	DataBase::SelectResult* curResult = (DataBase::SelectResult*) para;
	//д���ͷ
	if (curResult->columnNames.size() == 0)
	{//��ͷֻ��д��һ��
		for (int i = 0; i < nCount; i++)
		{
			curResult->columnNames.insert(curResult->columnNames.end(), pName[i]);
		}
	}
	//д������
	std::vector<std::string> curColumnValues;
	for (int i = 0; i < nCount; i++)
	{
		std::string curColumnValue = pValue[i] == NULL ? "" : pValue[i];//��¼����Ϣ��������������й���
		curColumnValues.insert(curColumnValues.end(), curColumnValue);
	}
	curResult->columnValues.insert(curResult->columnValues.end(), curColumnValues);
	curResult->count++;//ÿ�鵽һ�����������һ�Σ����ÿ�ν�������������Ľ������Ҫ++
	/*
	//���ÿ�β�ѯ���Ľ��
	std::string result;
	for (int i = 0; i < nCount; i++)
	{
	result = result + pName[i] + " = " + pValue[i] + " | ";
	}
	std::cout << result << std::endl;
	*/
	return 0;
}