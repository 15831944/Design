#pragma once
//����ЧӦ�࣬��ŵ��������������
//[XXT]ͷ�ļ���һ��ֻд������ģ�庯�������д����ע�⽫�����ŵ�struct��class����Ϊ��Ա���������򵱴�ͷ�ļ����������ʱ�������link�����ظ��������

#include "xxt.h"
#include <iostream>
#include <vector>
#include <set>
#include <map>

struct Force
{
	double N;
	double V2;
	double V3;
	double T;
	double M2;
	double M3;

	Force(double N, double V2, double V3, double T, double M2, double M3)
		:N(N), V2(V2), V3(V3), T(T), M2(M2), M3(M3)
	{}

	Force& operator=(const Force& f)
	{
		if (this != &f)
		{
			this->N = f.N;
			this->V2 = f.V2;
			this->V3 = f.V3;
			this->T = f.T;
			this->M2 = f.M2;
			this->M3 = f.M3;
		}
		return *this;
	}
	Force& operator+(const Force& f)
	{
		this->N += f.N;
		this->V2 += f.V2;
		this->V3 += f.V3;
		this->T += f.T;
		this->M2 += f.M2;
		this->M3 += f.M3;
		return *this;
	}
	Force& operator-(const Force& f)
	{
		this->N -= f.N;
		this->V2 -= f.V2;
		this->V3 -= f.V3;
		this->T -= f.T;
		this->M2 -= f.M2;
		this->M3 -= f.M3;
		return *this;
	}
	Force& operator*(double n)
	{
		this->N *= n;
		this->V2 *= n;
		this->V3 *= n;
		this->T *= n;
		this->M2 *= n;
		this->M3 *= n;
		return *this;
	}
	Force& operator*(const double n[])
	{//�˴�Ҫ��nΪ6��double��array
		this->N *= n[0];
		this->V2 *= n[1];
		this->V3 *= n[2];
		this->T *= n[3];
		this->M2 *= n[4];
		this->M3 *= n[5];
		return *this;
	}
	Force& operator/(double n)
	{
		if (abs(n - 0) <= EPSILON) std::cerr << "����Ϊ�㣡" << std::endl;
		this->N /= n;
		this->V2 /= n;
		this->V3 /= n;
		this->T /= n;
		this->M2 /= n;
		this->M3 /= n;
		return *this;
	}
	friend std::ostream& operator<<(std::ostream& os, const Force& f)//[XXT]��friendΪ��ʹ���������ΪForce����Ԫ����
	{//[XXT]�����������operator�ĳ�Ա����������struct��ʱ��Ҫ��friend������struct�������
		//[XXT]operator��unary_function(�涨����1������)��binary_function(�涨����2������)
		os << "N=" << f.N << std::endl;
		os << "V2=" << f.V2 << std::endl;
		os << "V3=" << f.V3 << std::endl;
		os << "T=" << f.T << std::endl;
		os << "M2=" << f.M2 << std::endl;
		os << "M3=" << f.M3 << std::endl;
		return os;
	}
};

//*------------------------------------*//
///��������
enum E_SingleCaseType//[]����������ö�ٷŵ�ForceEffort���У�δ��
{
	E_SCT_DEAD,//����
	E_SCT_LIVE,//����
	E_SCT_WIND,//����
	E_SCT_TEMPERATURE,//�¶�����
	E_SCT_PRESTRESS,//ԤӦ��
	E_SCT_AD,//�˷�����
	E_SCT_E//��������
};

struct CaseData
{
	Force force;//����
	E_SingleCaseType caseType;//�������
	CaseData(Force f, E_SingleCaseType cT)
		:force(f), caseType(cT)
	{}
	void setData(Force f, E_SingleCaseType cT)
	{
		this->force = f;
		this->caseType = cT;
	}
};

//*------------------------------------*//
///�������
enum E_CombinationType
{
	E_CT_CASE,//������
	E_CT_LOAD,//�ǵ������
	E_CT_SEISMIC,//�������
	E_CT_AD,//�˷����
	E_CT_NOMINAL,//��׼���
	E_CT_QP//׼�������
};

struct ForceData
{
	Force force;//����
	E_CombinationType combinationType;//�������
	ForceData(Force f, E_CombinationType cT)
		:force(f), combinationType(cT)
	{}
};

//*------------------------------------*//

class ForceEffect
{
public:
	ForceEffect();
	~ForceEffect();

	void setCaseMap(std::map<std::string, CaseData>* caseMap);//���õ���������
	void setFC(std::vector<std::string>* factorFC);//���û�����ϱ�
	void setNC(std::vector<std::string>* factorNC);//���ñ�׼��ϱ�
	void setQPC(std::vector<std::string>* factorQPC);//����׼������ϱ�
	bool hasFC(){ return m_FactorFC != NULL; }//����Ƿ����FC
	bool hasNC(){ return m_FactorNC != NULL; }//����Ƿ����NC
	bool hasQPC(){ return m_FactorQPC != NULL; }//����Ƿ����QPC
	void calcFC();//����������
	void calcNC();//�����׼���
	void calcQPC();//����׼�������
//[]һ�������б�Ҫ�ŵ�private����
	std::map<std::string, CaseData>* m_caseMap;//����������
	std::vector<std::string>* m_FactorFC;//�������ϵ����
	std::vector<std::string>* m_FactorNC;//��׼���ϵ����
	std::vector<std::string>* m_FactorQPC;//׼�������ϵ����
	std::vector<ForceData> m_FundamentalCombination;//�������
	std::vector<ForceData> m_NominalCombination;//��׼���
	std::vector<ForceData> m_QuasiPermanentCombination;//׼�������


private://���ڲ�ʹ�õĴ�Ҫ�Ӻ���
	void calcCombination//���ݺ������ϵ�����������������ɶ�Ӧ�ĺ������
		(std::vector<std::string>* factorTable
		, std::vector<ForceData>& combinationTable
		, int stage);//0 - �������, 1 - ��׼���, 2 - ׼�������
	ForceData calcCombineForce(const std::string line, int stage);//���ݾ����������ϱ��ʽ������������������Ϻ��������,0-�������,1-��׼���,2-׼�������
	void analyseCombination//��������ϱ��ʽ���ɴ�ϵ�����ֶΣ���1.2D��1.4L
		(const std::string& line
		, std::vector<std::string>& combination);
};

