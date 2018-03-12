#include "ConcreteElement.h"

#include <iostream>
#include <string>

ConcreteElement::ConcreteElement(){}

ConcreteElement::~ConcreteElement(){}

void ConcreteElement::setγ0(double γ0)//[XXT]静态成员函数写实现的时候不加static
{
	ConcreteElement::γ0 = γ0;//[XXT]静态成员变量不能用this
}

void ConcreteElement::setCaspMap(std::map<std::string, ForceEffect::E_SingleCaseType>* caseMap)
{
	ConcreteElement::m_caseMap = caseMap;
}

void ConcreteElement::setFactorFC_Str(std::vector<std::string>* factorFC)
{
	ConcreteElement::m_FactorFC_Str = factorFC;
	parserFactorFC();
}

void ConcreteElement::setFactorNC_Str(std::vector<std::string>* factorNC)
{
	ConcreteElement::m_FactorNC_Str = factorNC;
	parserFactorNC();
}

void ConcreteElement::setFactorQPC_Str(std::vector<std::string>* factorQPC)
{
	ConcreteElement::m_FactorQPC_Str = factorQPC;
	parserFactorQPC();
}

void ConcreteElement::parserFactorFC()
{
	parserCombination(m_FactorFC_Str, m_caseMap, m_FactorFC, ForceEffect::E_CombinationType::E_CT_FC);
}

void ConcreteElement::parserFactorNC()
{
	parserCombination(m_FactorNC_Str, m_caseMap, m_FactorNC, ForceEffect::E_CombinationType::E_CT_NC);
}

void ConcreteElement::parserFactorQPC()
{
	parserCombination(m_FactorQPC_Str, m_caseMap, m_FactorQPC, ForceEffect::E_CombinationType::E_CT_QPC);
}

void ConcreteElement::printAllFactorTables()
{
	std::cout << "基本组合" << std::endl;
	printFactorTable(m_FactorFC);
	std::cout << "标准组合" << std::endl;
	printFactorTable(m_FactorNC);
	std::cout << "准永久组合" << std::endl;
	printFactorTable(m_FactorQPC);
}

void ConcreteElement::printFactorTable(const std::vector<ForceEffect::CombineExp>& factorTable)
{
	for each(ForceEffect::CombineExp curCombineExp in factorTable)
	{
		for each(std::pair<double, std::string> curCombinePair in curCombineExp.casePairData)
		{
			std::cout << curCombinePair.first << "(" << curCombinePair.second << ")" << " + ";
		}
		switch (curCombineExp.combineType)
		{
		case ForceEffect::E_CombinationType::E_CT_FC:
			std::cout << "基本组合";
			break;
		case ForceEffect::E_CombinationType::E_CT_LOAD:
			std::cout << "非地震组合";
			break;
		case ForceEffect::E_CombinationType::E_CT_SEISMIC:
			std::cout << "地震组合";
			break;
		case ForceEffect::E_CombinationType::E_CT_AD:
			std::cout << "人防组合";
			break;
		case ForceEffect::E_CombinationType::E_CT_NC:
			std::cout << "标准组合";
			break;
		case ForceEffect::E_CombinationType::E_CT_QPC:
			std::cout << "准永久组合";
			break;
		default:
			std::cout << "未识别出组合！";
			break;
		}
		std::cout << std::endl;
	}
}

void ConcreteElement::parserCombination//根据 荷载组合系数表(string形式)、单工况类型表 生成对应的荷载组合系数表(解析后)
(std::vector<std::string>* factorTable_Str
, std::map<std::string, ForceEffect::E_SingleCaseType>*& caseMap
, std::vector<ForceEffect::CombineExp>& factorTable
, ForceEffect::E_CombinationType combineType)
{
	for each(std::string line in *factorTable_Str)
	{
		parserCombinationExp(line, factorTable, combineType);
	}
}

void ConcreteElement::parserCombinationExp//解析荷载组合表达式
(const std::string& line
, std::vector<ForceEffect::CombineExp>& factorTable
, ForceEffect::E_CombinationType combineType)
{
	ForceEffect::CombineExp::TD_CombinePair combineExp;//vector(单工况系数，单工况名称)
	ForceEffect::E_SingleCaseType maxSingleCaseType = ForceEffect::E_SingleCaseType::E_SCT_DEAD;//根据最大单工况类型判断荷载组合类型

	std::string caseName;//单工况名称
	double factor = 0;//单工况组合系数
	int indexStart = line.size() - 1;//抽取字符串起始位置
	int indexEnd = line.size() - 1;//抽取字符串结束位置
	bool gettingCaseName = true;//表示是否正在提取caseName
	bool gettingFactor = false;//表示是否正在提取factor
	//对line倒着进行解析
	for (int i = line.size() - 1; i >= 0; i--)
	{
		if (gettingCaseName && isdigit(line[i]))
		{//找到caseName起始位置
			gettingCaseName = false;
			gettingFactor = true;
			indexStart = i + 1;//更新caseName的startIndex
			caseName = line.substr(indexStart, indexEnd - indexStart + 1);
			caseName.erase(0, caseName.find_first_not_of(" "));//删除头部空格
			caseName.erase(caseName.find_last_not_of(" ") + 1);//删除尾部空格
			indexEnd = i;//准备提取factor
		}
		
		if (gettingFactor && (line[i] == '+' || line[i] == '-' || i == 0))
		{//找到factor起始位置			
			gettingCaseName = true;
			gettingFactor = false;
			indexStart = (i == 0) ? i : i + 1;//更新factor的startIndex
			std::string factorStr = line.substr(indexStart, indexEnd - indexStart + 1);
			factorStr.erase(0, factorStr.find_first_not_of(" "));//删除头部空格
			factorStr.erase(factorStr.find_last_not_of(" ") + 1);//删除尾部空格
			factor = std::stod(factorStr);
			if (line[i] == '-') factor = -factor;//判断是否系数为负数
			indexEnd = i -1;
			//将casePair写入combineExp中
			if (m_caseMap->count(caseName) == 1)
			{
				combineExp.insert(combineExp.begin(), std::pair<double, std::string>(factor, caseName));
				if (maxSingleCaseType < m_caseMap->at(caseName)) maxSingleCaseType = m_caseMap->at(caseName);
			}
			factor = 0;
		}
	}

	ForceEffect::E_CombinationType curCombinationType = getCombinationType(combineType, maxSingleCaseType);
	factorTable.insert(factorTable.end(), ForceEffect::CombineExp(curCombinationType, combineExp));
}

ForceEffect::E_CombinationType ConcreteElement::getCombinationType
(ForceEffect::E_CombinationType combineType
, ForceEffect::E_SingleCaseType maxSingleCaseType)
{
	switch (combineType)
	{
	case ForceEffect::E_CombinationType::E_CT_FC:
		switch (maxSingleCaseType)
		{
		case ForceEffect::E_SingleCaseType::E_SCT_AD:
			return ForceEffect::E_CombinationType::E_CT_AD;
			break;
		case ForceEffect::E_SingleCaseType::E_SCT_E:
			return ForceEffect::E_CombinationType::E_CT_SEISMIC;
			break;
		default:
			return ForceEffect::E_CombinationType::E_CT_LOAD;
			break;
		}
		break;
	case ForceEffect::E_CombinationType::E_CT_NC:
		return ForceEffect::E_CombinationType::E_CT_NC;
		break;
	case ForceEffect::E_CombinationType::E_CT_QPC:
		return ForceEffect::E_CombinationType::E_CT_QPC;
		break;
	default:
		std::cerr << "未知组合类型！" << std::endl;
		break;
	}
}

//*------------------------------------*//

Beam::Beam(E_BeamType beamType, int sectionNumber)//[XXT]写默认值时，只在声明中写variable = defaultValue，实现中只写variable
{//[XXT]构造函数给成员变量做“无意义”的初始化，给出“错误”的数值方便检查实例是否正常生成
	this->c = 999;
	this->Lc2 = 0;
	this->Lc3 = 0;
	setBeamType(beamType);
	setSectionNumber(sectionNumber);
	setMaterial(NULL, NULL, NULL, NULL);
	setCalculateParameter(E_NFB::E_NFB_NULL, E_NFB::E_NFB_NULL);
	setBeamType(E_BeamType::E_BT_BEAM);
	this->sections.resize(this->sectionNumber);
}

Beam::~Beam(){}

int Beam::getSectionNumber(){ return sectionNumber; }

void Beam::setBeamType(E_BeamType beamType)
{
	this->beamType = beamType;
}

void Beam::setSectionNumber(int sectionNumber)
{
	this->sectionNumber = sectionNumber;
}

void Beam::setSection
(const std::vector<Section*>& sections
//, const std::vector<BeamSection::E_BeamSectionLocation>& sectionLocationTypes
, double c
, double Lc2
, double Lc3)
{
	this->c = c;
	this->Lc2 = Lc2;
	this->Lc3 = Lc3;
	int inputSectionNum = sections.size();//传入sections的个数
	for(int i = 0; i < sectionNumber; i++)
	{
		if (i < inputSectionNum)
		{
			this->sections[i].setSection(sections[i]);
			this->sections[i].setSectionLocationType(BeamSection::E_BeamSectionLocation::E_BSL_LR);
			//this->sections[i].setSectionLocationType(sectionLocationTypes[i]);//[?]系统不认class BeamSection的前置声明？
		}
		else
		{
			this->sections[i].setSection(NULL);
		}
	}
}

void Beam::setMaterial(Concrete* concrete, Rebar* rebarL, Rebar* rebarS, Steel* steel)
{
	this->concrete = concrete;
	this->longitudinal = rebarL;
	this->stirrup = rebarS;
	this->skeleton = steel;
}

void Beam::setCalculateParameter(E_NFB Nfb, E_NFB Nfb_gz)
{
	this->Nfb = Nfb;
	this->Nfb_gz = Nfb_gz;
}

void Beam::setCaseMap(const std::vector<std::map<std::string, Force>>& caseMaps)
{
	int caseMapsSize = caseMaps.size();
	for (int i = 0; i < sectionNumber; i++)
	{
		if (i < caseMapsSize) this->sections[i].setCombineForceData(caseMaps[i], &this->m_FactorFC, &this->m_FactorNC, &this->m_FactorQPC);
	}
}

void Beam::calcCombineForceData()
{
	for (int i = 0; i < sectionNumber; i++)
	{
		sections[i].calcCombineForceData();
	}
}

void Beam::showResult()
{
	for (int i = 0; i < sections.size(); i++)
	{
		std::cout << std::string(10, '-') << "第" << i << "号截面设计结果：" << std::string(10, '-') << std::endl;
		sections[i].showResult();
	}
}

//*------------------------------------*//

BeamSection::BeamSection()
{
	setSection(NULL);
	setSectionLocationType(E_BeamSectionLocation::E_BSL_UNKNOWN);
}

BeamSection::~BeamSection(){}

void BeamSection::setSection(Section* section)
{
	this->section = section;
}

void BeamSection::setSectionLocationType(E_BeamSectionLocation sectionLocation)
{
	this->sectionLocation = sectionLocation;
}

void BeamSection::setCombineForceData
(const std::map<std::string, Force>& caseMap
, std::vector<ForceEffect::CombineExp>* factorFC
, std::vector<ForceEffect::CombineExp>* factorNC
, std::vector<ForceEffect::CombineExp>* factorQPC
){
	forceData.setCaseMap(caseMap);
	forceData.setFC(factorFC);
	forceData.setNC(factorNC);
	forceData.setQPC(factorQPC);
}

void BeamSection::calcCombineForceData()
{
	if (forceData.hasFC()) forceData.calcFC();
	if (forceData.hasNC()) forceData.calcNC();
	if (forceData.hasQPC()) forceData.calcQPC();
}

void BeamSection::showResult()
{
	for (int i = 0; i < m_resultFC.size(); i++)
	{
		std::cout << "第" << i << "号组合结果:" << std::endl;
		std::cout << "正截面 M3=" << forceData.m_FundamentalCombination[i].force.M3 << std::endl;
		std::cout << "x=" << m_resultFC[i].x << std::endl;
		std::cout << "As=" << m_resultFC[i].As << "  ρ=" << m_resultFC[i].ρ << std::endl;
		std::cout << "As'=" << m_resultFC[i].As_c << "  ρ'=" << m_resultFC[i].ρc << std::endl;
		std::cout << "斜截面" << std::endl;
		std::cout << "Asv=" << m_resultFC[i].Asv << "  ρsv=" << m_resultFC[i].ρsv << std::endl;
		std::cout << std::endl;
	}
}