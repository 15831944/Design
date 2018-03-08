#include "force_effect.h"
#include "iostream"
#include <sstream>

//[prototype]
///生成荷载组合
void calcCombination//[]这个函数只给cpp用，需要放到类里作为private吗？
	(std::vector<std::string>* factorTable
	, std::vector<ForceData>& combinationTable);

///进行荷载组合
Force1 generateCombination
	(const std::map<std::string, CaseData>& m_caseMap
	, const std::string line);
///分析组合表达式
void analyseCombination
	(const std::string& line
	, std::vector<std::string>& combination);

ForceEffect::ForceEffect()
{
	m_FactorFC = NULL;
	m_FactorNC = NULL;
	m_FactorQPC = NULL;
}


ForceEffect::~ForceEffect()
{
}

void ForceEffect::setCaseMap(std::map<std::string, CaseData>* caseMap)
{
	this->m_caseMap = caseMap;
}

void ForceEffect::setFC(std::vector<std::string>* factorFC)
{
	this->m_FactorFC = factorFC;
}

void ForceEffect::setNC(std::vector<std::string>* factorNC)
{
	this->m_FactorNC = factorNC;
}

void ForceEffect::setQPC(std::vector<std::string>* factorQPC)
{
	this->m_FactorQPC = factorQPC;
}

void ForceEffect::calcFC()
{
	calcCombination(m_FactorFC, m_FundamentalCombination);
}

void ForceEffect::calcNC()
{
	calcCombination(m_FactorNC, m_NominalCombination);
}

void ForceEffect::calcQPC()
{
	calcCombination(m_FactorQPC, m_QuasiPermanentCombination);
}

void calcCombination
	(const std::map<std::string, CaseData>& caseMap
	, std::vector<std::string>* factorTable
	, std::vector<ForceData>& combinationTable)
{
	for each(std::string  line in *factorTable){
		//[]这里改成传出forcedata类型，里面计算好组合类型
		Force1 curForce = generateCombination(caseMap, line);
		combinationTable.insert(combinationTable.end(), ForceData(curForce, E_CombinationType::E_CT_QP));
	}
}

Force1 generateCombination//[]需要添加识别组合类型的函数
	(const std::map<std::string, CaseData>& m_caseMap
	, const std::string line)
{
	std::vector<std::string> combination;//带系数的字段，如1.2D
	analyseCombination(line, combination);
	Force1 result = Force1(0, 0, 0, 0, 0, 0);//组合内力
	for each(std::string segment in combination)
	{
		std::stringstream ss;
		ss << segment;
		double factor = 0;
		std::string caseName;
		ss >> factor >> caseName;
		if (m_caseMap.count(caseName) == 1)
		{
			Force1 curForce = m_caseMap.at(caseName).force;
			result = result + curForce * factor;
		}
	}
	return result;
}

void analyseCombination
	(const std::string& line
	, std::vector<std::string>& combination)
{
	std::string segment;
	for (int i = 0; i < line.size(); i++)
	{
		switch (line[i])
		{
		case '+':
			combination.insert(combination.end(), segment);
			segment.clear();
			break;
		case '-':
			combination.insert(combination.end(), segment);
			segment = "-";
			break;
		default:
			segment += line[i];
			break;
		}
	}
	combination.insert(combination.end(), segment);
}