#include <iostream>

#include "beam.h"//[]以后改名字

ConcreteElement::ConcreteElement(){}

ConcreteElement::~ConcreteElement(){}

void ConcreteElement::setγ0(double γ0)//[XXT]静态成员函数写实现的时候不加static
{
	ConcreteElement::γ0 = γ0;//[]静态成员变量不能用this吧？
}

void ConcreteElement::setFactorFC(std::vector<std::string>* factorFC)
{
	ConcreteElement::m_FactorFC = factorFC;
}

void ConcreteElement::setFactorNC(std::vector<std::string>* factorNC)
{
	ConcreteElement::m_FactorNC = factorNC;
}

void ConcreteElement::setFactorQPC(std::vector<std::string>* factorQPC)
{
	ConcreteElement::m_FactorQPC = factorQPC;
}

//*------------------------------------*//

Beam::Beam()
{
	this->c = 999;
	this->Lc2 = 0;
	this->Lc3 = 0;
	setMaterial(NULL, NULL, NULL, NULL);
	setCalculateParameter(E_NFB::E_NFB_NULL, E_NFB::E_NFB_NULL);
	setBeamType(E_BeamType::E_BT_BEAM);
	this->sections.resize(SECTION_NUMBER);
}

Beam::~Beam(){}

void Beam::setBeamType(E_BeamType beamType)
{
	this->beamType = beamType;
}

void Beam::setSection(const std::vector<Section*>& sections, double c, double Lc2, double Lc3)
{
	this->c = c;
	this->Lc2 = Lc2;
	this->Lc3 = Lc3;
	for(int i = 0; i < sections.size(); i++)
	{
		this->sections[i].setSection(sections[i]);
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

void Beam::setCaseMap(const std::vector<std::map<std::string, CaseData>>& caseMaps)
{
	for (int i = 0; i < sections.size(); i++)
	{
		this->sections[i].setForceData(caseMaps[i], this->m_FactorFC, this->m_FactorNC, this->m_FactorQPC);
	}
}

void Beam::calcForceData()
{
	for (int i = 0; i < sections.size(); i++)
	{
		sections[i].calcForceData();
	}
}

void Beam::showResult()
{
	for (int i = 0; i < sections.size(); i++)
	{
		std::cout << "第" << i << "号截面设计结果：" << std::endl;
		sections[i].showResult();
	}
}

//*------------------------------------*//

BeamSection::BeamSection()
{
	//setSection(NULL);
	//setSectionLocationType(E_BeamSectionLocation::E_BSL_NULL);//[]能将枚举初始化成NULL吗？
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

void BeamSection::setForceData
(const std::map<std::string, CaseData>& caseMap
, std::vector<std::string>* factorFC
, std::vector<std::string>* factorNC
, std::vector<std::string>* factorQPC
){
	forceData.setCaseMap(caseMap);
	forceData.setFC(factorFC);
	forceData.setNC(factorNC);
	forceData.setQPC(factorQPC);
}

void BeamSection::calcForceData()
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
		std::cout << "正截面" << std::endl;
		std::cout << "x=" << m_resultFC[i].x << std::endl;
		std::cout << "As=" << m_resultFC[i].As << "  ρ=" << m_resultFC[i].ρ << std::endl;
		std::cout << "As'=" << m_resultFC[i].As_c << "  ρ'=" << m_resultFC[i].ρc << std::endl;
		std::cout << "斜截面" << std::endl;
		std::cout << "Asv=" << m_resultFC[i].Asv << "  ρsv=" << m_resultFC[i].ρsv << std::endl;
		std::cout << std::endl;
	}
}