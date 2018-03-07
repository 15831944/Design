#include "force_effect.h"
#include "iostream"
#include <sstream>

//[prototype]
///荷载组合
Force1 generateCombination
	(const std::map<std::string, Force1>& m_caseMap
	, const std::string line
	);
///分析组合表达式
void analyseCombination
	(const std::string& line
	, std::vector<std::string>& combination
	);

ForceEffect::ForceEffect()
{
}


ForceEffect::~ForceEffect()
{
}

void ForceEffect::setFC(std::vector<std::string>* factorFC)
{
	this->m_FactorFC = factorFC;
	for each(std::string  line in *m_FactorFC){
		Force1 curForce = generateCombination(m_caseMap, line);
		m_FundamentalCombination.insert(m_FundamentalCombination.end(), ForceData(curForce, E_CombinationType::E_CT_LOAD));
	}
}

Force1 generateCombination
(const std::map<std::string, Force1>& m_caseMap
, const std::string line
){
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
			Force1 curForce = m_caseMap.at(caseName);
			result = result + curForce * factor;
		}
	}
	return result;
}

void analyseCombination
(const std::string& line
, std::vector<std::string>& combination
){
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