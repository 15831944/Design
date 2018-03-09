#include <iostream>

#include "beam.h"//[]�Ժ������

ConcreteElement::ConcreteElement(){}

ConcreteElement::~ConcreteElement(){}

void ConcreteElement::set��0(double ��0)//[XXT]��̬��Ա����дʵ�ֵ�ʱ�򲻼�static
{
	ConcreteElement::��0 = ��0;//[]��̬��Ա����������this�ɣ�
};

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
	this->c = 20;
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

void Beam::setSection(const std::vector<Section*>& sections, double c)
{
	this->c = c;
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
	for each (BeamSection curSection in sections)
	{
		curSection.calcForceData();
	}
}

void Beam::showResult()
{
	for each (BeamSection curSection in sections)
	{
		curSection.showResult();
	}
}

//*------------------------------------*//

BeamSection::BeamSection()
{
	setSection(NULL);
	setSectionType(E_BeamSectionType::E_BST_NULL);//[]��ö�ٳ�ʼ����NULL����
}

BeamSection::~BeamSection(){}

void BeamSection::setSectionType(E_BeamSectionType sectionType)
{
	this->sectionType = sectionType;
}

void BeamSection::setSection(Section* section)
{
	this->section = section;
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
	for (int i = 0; i < m_result.size(); i++)
	{
		std::cout << "��" << i << "����Ͻ��:" << std::endl;
		std::cout << "������" << std::endl;
		std::cout << "x=" << m_result[i].x << std::endl;
		std::cout << "As=" << m_result[i].As << "  ��=" << m_result[i].�� << std::endl;
		std::cout << "As'=" << m_result[i].As_c << "  ��'=" << m_result[i].��c << std::endl;
		std::cout << "б����" << std::endl;
		std::cout << "Asv=" << m_result[i].Asv << "  ��sv=" << m_result[i].��sv << std::endl;
		std::cout << std::endl;
	}
}