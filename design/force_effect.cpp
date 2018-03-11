#include <iostream>
#include <string>

#include "force_effect.h"

ForceEffect::ForceEffect()
{
	m_FactorFC = NULL;
	m_FactorNC = NULL;
	m_FactorQPC = NULL;
}

ForceEffect::~ForceEffect(){}

void ForceEffect::setCaseMap(const std::map<std::string, CaseData>& caseMap)
{
	this->m_caseMap = caseMap;
}

void ForceEffect::setFC(std::vector<CombineData>* factorFC)
{
	this->m_FactorFC = factorFC;
}

void ForceEffect::setNC(std::vector<CombineData>* factorNC)
{
	this->m_FactorNC = factorNC;
}

void ForceEffect::setQPC(std::vector<CombineData>* factorQPC)
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
(const std::vector<CombineData>* factorTable//���ϵ����(������)
, std::vector<ForceData>& combinationTable)//��Ͻ����
{
	for each(CombineData curCombineData in *factorTable)
	{//�������ϵ��������caseMap���ɶ�Ӧ����Ͻ����
		ForceData curForceData = ForceData(calcCombineForce(curCombineData), curCombineData.combineType);
		combinationTable.insert(combinationTable.end(), curForceData);
	}
}

Force ForceEffect::calcCombineForce(const CombineData& curCombineData)
{
	Force result = Force(0, 0, 0, 0, 0, 0);
	for each(std::pair<double, std::string> factorPair in curCombineData.combinePairData)
	{
		if (m_caseMap.count(factorPair.second) == 1)
		{
			Force curForce = m_caseMap.at(factorPair.second).force;
			result = result + curForce * factorPair.first;
		}
	}
	return result;
}