#pragma once
//作用效应类，存放单工况、组合内力
//[XXT]头文件里一般只写声明、模板函数，如果写函数注意将函数放到struct或class中作为成员函数，否则当此头文件被多次引用时，会产生link错误（重复定义错误）

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
	{//此处要求n为6个double的array
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
		if (abs(n - 0) <= EPSILON) std::cerr << "除数为零！" << std::endl;
		this->N /= n;
		this->V2 /= n;
		this->V3 /= n;
		this->T /= n;
		this->M2 /= n;
		this->M3 /= n;
		return *this;
	}
	friend std::ostream& operator<<(std::ostream& os, const Force& f)//[XXT]加friend为了使这个函数成为Force的友元函数
	{//[XXT]这个函数属于operator的成员函数，用在struct里时需要加friend来访问struct里的内容
		//[XXT]operator有unary_function(规定传入1个变量)和binary_function(规定传入2个变量)
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
///工况类型
enum E_SingleCaseType//[]本来想把这个枚举放到ForceEffort类中，未遂
{
	E_SCT_DEAD,//恒载
	E_SCT_LIVE,//活载
	E_SCT_WIND,//风载
	E_SCT_TEMPERATURE,//温度作用
	E_SCT_PRESTRESS,//预应力
	E_SCT_AD,//人防荷载
	E_SCT_E//地震作用
};

struct CaseData
{
	Force force;//内力
	E_SingleCaseType caseType;//组合类型
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
///组合类型
enum E_CombinationType
{
	E_CT_CASE,//单工况
	E_CT_LOAD,//非地震组合
	E_CT_SEISMIC,//地震组合
	E_CT_AD,//人防组合
	E_CT_NOMINAL,//标准组合
	E_CT_QP//准永久组合
};

struct ForceData
{
	Force force;//内力
	E_CombinationType combinationType;//组合类型
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

	void setCaseMap(std::map<std::string, CaseData>* caseMap);//设置单工况内力
	void setFC(std::vector<std::string>* factorFC);//设置基本组合表
	void setNC(std::vector<std::string>* factorNC);//设置标准组合表
	void setQPC(std::vector<std::string>* factorQPC);//设置准永久组合表
	bool hasFC(){ return m_FactorFC != NULL; }//检测是否包含FC
	bool hasNC(){ return m_FactorNC != NULL; }//检测是否包含NC
	bool hasQPC(){ return m_FactorQPC != NULL; }//检测是否包含QPC
	void calcFC();//计算基本组合
	void calcNC();//计算标准组合
	void calcQPC();//计算准永久组合
//[]一下内容有必要放到private里吗？
	std::map<std::string, CaseData>* m_caseMap;//单工况内力
	std::vector<std::string>* m_FactorFC;//基本组合系数表
	std::vector<std::string>* m_FactorNC;//标准组合系数表
	std::vector<std::string>* m_FactorQPC;//准永久组合系数表
	std::vector<ForceData> m_FundamentalCombination;//基本组合
	std::vector<ForceData> m_NominalCombination;//标准组合
	std::vector<ForceData> m_QuasiPermanentCombination;//准永久组合


private://仅内部使用的次要子函数
	void calcCombination//根据荷载组合系数表、单工况内力生成对应的荷载组合
		(std::vector<std::string>* factorTable
		, std::vector<ForceData>& combinationTable
		, int stage);//0 - 基本组合, 1 - 标准组合, 2 - 准永久组合
	ForceData calcCombineForce(const std::string line, int stage);//根据具体的内力组合表达式、单工况内力生成组合后内力结果,0-基本组合,1-标准组合,2-准永久组合
	void analyseCombination//将内力组合表达式拆解成带系数的字段，如1.2D、1.4L
		(const std::string& line
		, std::vector<std::string>& combination);
};

