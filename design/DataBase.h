#pragma once

#include <string>
#include <vector>
#include <map>

#include "sqlite3.h"

static const std::string NO_VALUE = "NULL";//��ʶ����Ϊ��//[]��ôд�����

class DataBase
{
public://ͨ������
	enum E_SQL_Variable_Type
	{
		E_SQLVT_NULL,//ֵ��һ�� NULL ֵ
		E_SQLVT_INTEGER,//ֵ��һ�������ŵ�����������ֵ�Ĵ�С�洢�� 1��2��3��4��6 �� 8 �ֽ���
		E_SQLVT_REAL,//ֵ��һ������ֵ���洢Ϊ 8 �ֽڵ� IEEE ��������
		E_SQLVT_TEXT,//ֵ��һ���ı��ַ�����ʹ�����ݿ���루UTF-8��UTF-16BE �� UTF-16LE���洢
		E_SQLVT_BLOB//ֵ��һ�� blob ���ݣ���ȫ������������洢������ֱ�Ӵ洢ͼ����Ƶ��
	};
	
	struct SelectResult
	{
		int count;//��ѯ��������������columnValues�ĵ�һ������
		std::vector<std::string> columnNames;//������,��ͷ
		std::vector<std::vector<std::string>> columnValues;//�б�ֵ
		SelectResult()
		{
			this->clear();
		}
		void clear()
		{
			this->count = 0;
			columnNames.clear();
			columnValues.clear();
		}
	};

	static const int COLUMN_WIDTH_INDEX = 3;//���������ſ��
	static const int COLUMN_WIDTH = 10;//������п�
public:
	DataBase();
	~DataBase();

	void setPath(std::string);//����·��
	std::string getPath();//��ȡ·��
	void setShowLog(bool showLog);//�����Ƿ������־,Ĭ�Ͽ���
	void open(bool inMemory = false);//�����ݿ�,Ĭ�ϴ��ļ���,T-�ڴ����ݿ�,F-�ļ����ݿ�
	int close();//�ر����ݿ�
	bool createTable//�������
		(const std::string& sTableName
		, std::vector<std::string> columnNameAndTypes//column����
		, std::vector<std::string> primaryKeys = std::vector<std::string>{NO_VALUE});//��������
	bool deleteTable(const std::string& sTableName);//ɾ�����

	bool addRow//�����
		(const std::string& sTableName
		, const std::vector<std::string>& columnValues//����еĸ�������
		, const std::vector<std::string>& columnNames = std::vector<std::string>{NO_VALUE});//��columnNames����λ���columnValues
	bool deleteRow//ɾ�������ض���������[��д��ɾ��������]
		(const std::string& sTableName
		, const std::string& sCondition = NO_VALUE);//����[��ѡ]
	bool setRow//�޸������ݣ����Ը���WHERE���[��д���޸��е���������]
		(const std::string& sTableName
		, const std::map<std::string, std::string> columnNameValuePairs//Ҫ�޸ĵ�������,columnName=columnValue
		, const std::string& sCondition = NO_VALUE);//����[��ѡ]
	bool selectColumn//ѡ���У����Ը���WHERE���[��д��ѡ���������]
		(const std::string& sTableName
		, const std::vector<std::string> columnNames = std::vector<std::string>{NO_VALUE}//Ҫѡ���������
		, const std::string& sCondition = NO_VALUE);//����[��ѡ]
	void printSelectResult();//���ѡ����
	void printTable(std::string sTableName);//������

private:
	std::string path;//[]���ݿ��ļ�·��,����utf-8����,���·���а�����������Ҫ���б���ת��,path=""ʱΪ�ڴ����ݿ�
	sqlite3* ptDB;//�����ϵ����ݿ�
	int nRes;//���н��״̬,0-��������
	std::string strSql;//SQL���
	std::string message;//����executeCommand����ʾ��ϢcommandName
	char* cErrMsg;//������Ϣ
	SelectResult* selectResult = new SelectResult();//SELECT���Ĳ�ѯ���
	bool showLog;//�Ƿ����log,Ĭ�����

	bool executeCommand//ִ��SQL����
		(std::string commandName
		, sqlite3* ptDB//���ݿ�ʵ��(ָ��)
		, std::string sqlCommand//SQL����
		, int(*callback)(void* selectResult, int, char**, char**) = NULL//[��ѡ]�ص�����//[?]����ʲô�÷���ֻ������û�б�����
		, void* call_Back_1th_parameter = NULL);//[��ѡ]��Ϊ�ص�����callback�ĵ�һ������,���Դ�ͨ��sqlite3_exec()��void*����callback()ʹ�ã��������¼��ѯ����ȣ�
};

