#include "ForceEffect.h"

#include <iostream>
#include <string>

ForceEffect::ForceEffect()
{
	m_FactorFC = NULL;
	m_FactorNC = NULL;
	m_FactorQPC = NULL;
}

ForceEffect::~ForceEffect(){}

void ForceEffect::setCaseMap(const std::map<std::string, CaseForceData>& caseMap)
{
	this->m_caseMap = caseMap;
}

void ForceEffect::setFC(std::vector<CombineExp>* factorFC)
{
	this->m_FactorFC = factorFC;
}

void ForceEffect::setNC(std::vector<CombineExp>* factorNC)
{
	this->m_FactorNC = factorNC;
}

void ForceEffect::setQPC(std::vector<CombineExp>* factorQPC)
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

void ForceEffect::calcCombination
(const std::vector<CombineExp>* factorTable//组合系数表(解析后)
, std::vector<CombineForceData>& combinationTable)//组合结果表
{
	for each(CombineExp curCombineExp in *factorTable)
	{//遍历组合系数表，根据caseMap生成对应的组合结果表
		CombineForceData curForceData = CombineForceData(calcCombineForce(curCombineExp), curCombineExp.combineType);
		combinationTable.insert(combinationTable.end(), curForceData);
	}
}

Force ForceEffect::calcCombineForce(const CombineExp& curCombineExp)
{
	Force result = Force(0, 0, 0, 0, 0, 0);
	for each(std::pair<double, std::string> factorPair in curCombineExp.casePairData)
	{
		if (m_caseMap.count(factorPair.second) == 1)
		{
			Force curForce = m_caseMap.at(factorPair.second).force;
			result = result + curForce * factorPair.first;
		}
	}
	return result;
}