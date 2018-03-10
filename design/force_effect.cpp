#include <iostream>
#include <sstream>

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
	calcCombination(m_FactorFC, m_FundamentalCombination, 0);
}

void ForceEffect::calcNC()
{
	calcCombination(m_FactorNC, m_NominalCombination, 1);
}

void ForceEffect::calcQPC()
{
	calcCombination(m_FactorQPC, m_QuasiPermanentCombination, 2);
}

void ForceEffect::calcCombination
	(std::vector<std::string>* factorTable
	, std::vector<ForceData>& combinationTable
	, int stage)//0 - �������, 1 - ��׼���, 2 - ׼�������
{
	for each(std::string  line in *factorTable){
		ForceData curForceData = calcCombineForce(line, stage);//���ݵ�i��������ϱ��ʽline�͵������������ɶ�Ӧ���������
		combinationTable.insert(combinationTable.end(), curForceData);
	}
}

ForceData ForceEffect::calcCombineForce(const std::string line, int stage)//[]��Ҫ���ʶ��������͵ĺ���
{
	std::vector<std::string> combination;//��line��ɴ�ϵ�����ֶΣ���1.2D
	analyseCombination(line, combination);
	ForceData result = ForceData(Force(0, 0, 0, 0, 0, 0), E_CombinationType::E_CT_LOAD);//�������
	E_SingleCaseType maxSingleCaseType = E_SingleCaseType::E_SCT_DEAD;
	for each(std::string segment in combination)
	{
		std::stringstream ss;
		ss << segment;
		double factor = 0;
		std::string caseName;
		ss >> factor >> caseName;//[]stringstream������ĸeҲʶ��Ϊ������
		if (m_caseMap.count(caseName) == 1)
		{
			CaseData curCaseData = m_caseMap.at(caseName);
			if (maxSingleCaseType < curCaseData.caseType) maxSingleCaseType = curCaseData.caseType;
			result.force = result.force + curCaseData.force * factor;
		}
	}
	switch (stage)
	{
	case 0:
		switch (maxSingleCaseType)
		{
		case E_SingleCaseType::E_SCT_AD:
			result.combinationType = E_CombinationType::E_CT_AD;
			break;
		case E_SingleCaseType::E_SCT_E:
			result.combinationType = E_CombinationType::E_CT_SEISMIC;
			break;
		default:
			result.combinationType = E_CombinationType::E_CT_LOAD;
			break;
		}
		break;
	case 1:
		result.combinationType = E_CombinationType::E_CT_NOMINAL;
		break;
	case 2:
		result.combinationType = E_CombinationType::E_CT_QP;
		break;
	default:
		std::cerr << "δ֪������ͣ�" << std::endl;
		break;
	}
	return result;
}

void ForceEffect::analyseCombination
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