#pragma once

#include <string>
#include <vector>

#include "sqlite3.h"

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

public:
	DataBase();
	~DataBase();

	void setPath(std::string);//����·��
	std::string getPath();//��ȡ·��
	void open();//�����ݿ�
	int close();//�ر����ݿ�
	bool createTable
		(const std::string& sTableName
		, std::vector<std::string> columnNameAndTypes);//�������
	bool addUser(const std::string& sName, const std::string& sAge);//�����
	bool deleteUser(const std::string& sName);//ɾ����
	bool modifyUser(const std::string& sName, const std::string& sAge);//�޸���

private:
	std::string path;//[]���ݿ��ļ�·��������utf-8���룬���·���а�����������Ҫ���б���ת��
	sqlite3* ptDB;//�����ϵ����ݿ�
	int stage;//���ݿ�״̬
	char* cErrMsg;//������Ϣ

	bool executeCommand//ִ��SQL����
		(std::string commandName
		, sqlite3* ptDB//���ݿ�ʵ��(ָ��)
		, std::string sqlCommand//SQL����
		, int(*callback)(void*, int, char**, char**)//�ص�����//[?]����ʲô�÷���
		, void *);//��Ϊ�ص������ĵ�һ������//[?]������������д��������

	static int callback//[?]��֪����ʲô�õģ������Ǹ�sqlite3_exec�ĵ�3�������õ�
		(void *NotUsed//�� sqlite3_exec() �ĵ��ĸ��������ݶ���
		, int argc//�������
		, char **argv//ָ���ѯ�����ָ������, ������ sqlite3_column_text() �õ�
		, char **azColName);//ָ���ͷ����ָ������, ������ sqlite3_column_name() �õ�

};

