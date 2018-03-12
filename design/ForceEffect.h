#pragma once
//����ЧӦ�࣬��ŵ��������������
//[XXT]ͷ�ļ���һ��ֻд������ģ�庯�������д����ע�⽫�����ŵ�struct��class����Ϊ��Ա���������򵱴�ͷ�ļ����������ʱ�������link�����ظ��������

#include <iostream>
#include <vector>
#include <map>

#include "xxt.h"

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

class ForceEffect
{
	friend class Design;
	friend class DesignBeam;//[?]friend��μ̳У�
	friend class BeamSection;
public://ͨ������
	enum E_SingleCaseType
	{//����������
		E_SCT_DEAD,//����
		E_SCT_LIVE,//����
		E_SCT_WIND,//����
		E_SCT_TEMPERATURE,//�¶�����
		E_SCT_PRESTRESS,//ԤӦ��
		E_SCT_AD,//�˷�����
		E_SCT_E//��������
	};

	enum E_CombinationType
	{///�������
		E_CT_CASE,//������
		E_CT_LOAD,//�ǵ������
		E_CT_SEISMIC,//�������
		E_CT_AD,//�˷����
		E_CT_FC,//�������
		E_CT_NC,//��׼���
		E_CT_QPC//׼�������
	};

	struct CombineExp
	{//һ��������Ͻ����������
		typedef std::vector<std::pair<double, std::string>> TD_CombinePair;
		ForceEffect::E_CombinationType combineType;//�������
		TD_CombinePair casePairData;//(ϵ��������������)������
		CombineExp(){ combineType = ForceEffect::E_CombinationType::E_CT_CASE; }
		CombineExp(ForceEffect::E_CombinationType cT, TD_CombinePair cD)
			:combineType(cT), casePairData(cD)
		{}
	};

	struct CombineForceData
	{//�����������
		Force force;//����
		E_CombinationType combinationType;//�������
		CombineForceData(Force f, E_CombinationType cT)
			:force(f), combinationType(cT)
		{}
	};

public:
	ForceEffect();
	~ForceEffect();

	void setCaseMap(const std::map<std::string, Force>& caseMap);//���õ���������
	void setFC(std::vector<ForceEffect::CombineExp>* factorFC);//���û�����ϱ�
	void setNC(std::vector<ForceEffect::CombineExp>* factorNC);//���ñ�׼��ϱ�
	void setQPC(std::vector<ForceEffect::CombineExp>* factorQPC);//����׼������ϱ�
	bool hasFC(){ return m_FactorFC != NULL; }//����Ƿ����FC
	bool hasNC(){ return m_FactorNC != NULL; }//����Ƿ����NC
	bool hasQPC(){ return m_FactorQPC != NULL; }//����Ƿ����QPC
	void calcFC();//����������
	void calcNC();//�����׼���
	void calcQPC();//����׼�������

private:
	std::map<std::string, Force> m_caseMap;//����������
	std::vector<ForceEffect::CombineForceData> m_FundamentalCombination;//�������
	std::vector<ForceEffect::CombineForceData> m_NominalCombination;//��׼���
	std::vector<ForceEffect::CombineForceData> m_QuasiPermanentCombination;//׼�������
	std::vector<ForceEffect::CombineExp>* m_FactorFC;//�������ϵ����(������)
	std::vector<ForceEffect::CombineExp>* m_FactorNC;//��׼���ϵ����(������)
	std::vector<ForceEffect::CombineExp>* m_FactorQPC;//׼�������ϵ����(������)

private://��Ҫ�ڲ�����
	void calcCombination//���� �������ϵ����(������)������������ ���ɶ�Ӧ�ĺ������
		(const std::vector<ForceEffect::CombineExp>* factorTable
		, std::vector<ForceEffect::CombineForceData>& combinationTable);
	Force calcCombineForce(const ForceEffect::CombineExp& curCombineExp);//�������ϵ����caseMap���ɶ�Ӧ����Ͻ����

};