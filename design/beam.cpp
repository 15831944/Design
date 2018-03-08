#include <iostream>

#include "beam.h"

Beam::Beam()
{ 
	setSection(NULL, 20);
	setMaterial(NULL, NULL, NULL, NULL);
	setCalculateParameter(1, E_NFB::E_NFB_NULL, E_NFB::E_NFB_NULL);
	setBeamType(E_BeamType::E_BT_BEAM);
}

Beam::~Beam(){}

void Beam::setSection(Section* section, double c)
{
	this->section = section;
	this->c = c;
}

void Beam::setMaterial(Concrete* concrete, Rebar* rebarL, Rebar* rebarS, Steel* steel)
{
	this->concrete = concrete;
	this->longitudinal = rebarL;
	this->stirrup = rebarS;
	this->skeleton = steel;
}

void Beam::setCalculateParameter(double γ0, E_NFB Nfb, E_NFB Nfb_gz)
{
	this->γ0 = γ0;
	this->Nfb = Nfb;
	this->Nfb_gz = Nfb_gz;
}

void Beam::setBeamType(E_BeamType beamType)
{
	this->beamType = beamType;
}

void Beam::setForceData
(std::map<std::string, CaseData>* caseMap
, std::vector<std::string>* factorFC
, std::vector<std::string>* factorNC
, std::vector<std::string>* factorQPC
){
	forceData.setCaseMap(caseMap);
	forceData.setFC(factorFC);
	forceData.setNC(factorNC);
	forceData.setQPC(factorQPC);
}

void Beam::calcForceData()
{
	if (forceData.hasFC()) forceData.calcFC();
	if (forceData.hasNC()) forceData.calcNC();
	if (forceData.hasQPC()) forceData.calcQPC();
}

void Beam::showResult()
{
	for (int i = 0; i < m_result.size(); i++)
	{
		std::cout << "第" << i << "号组合结果:" << std::endl;
		std::cout << "正截面" << std::endl;
		std::cout << "x=" << m_result[i].x << std::endl;
		std::cout << "As=" << m_result[i].As << "  ρ=" << m_result[i].ρ << std::endl;
		std::cout << "As'=" << m_result[i].As_c << "  ρ'=" << m_result[i].ρc << std::endl;
		std::cout << "斜截面" << std::endl;
		std::cout << "Asv=" << m_result[i].Asv << "  ρsv=" << m_result[i].ρsv << std::endl;
		std::cout << std::endl;
	}
}