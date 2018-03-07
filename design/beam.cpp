#include <iostream>

#include "beam.h"

Beam::Beam()
{ 
	setSection(NULL, 20);
	setMaterial(NULL, NULL, NULL, NULL);
	setCalculateParameter(0, 5, 5);
	setBeamType(E_BeamType::E_BT_BEAM);
	setCheck();
}

Beam::~Beam(){}

void Beam::setSection(Section* section, double c)
{
	data.section = section;
	data.c = c;
}

void Beam::setMaterial(Concrete* concrete, Rebar* rebarL, Rebar* rebarS, Steel* steel)
{
	data.concrete = concrete;
	data.longitudinal = rebarL;
	data.stirrup = rebarS;
	data.skeleton = steel;
}

void Beam::setCalculateParameter(double ��0, int Nfb, int Nfb_gz)
{
	data.��0 = ��0;
	data.Nfb = Nfb;
	data.Nfb_gz = Nfb_gz;
}

void Beam::setBeamType(E_BeamType beamType)
{
	data.beamType = beamType;
}

void Beam::setForce(double N, double V2, double V3, double T, double M2, double M3)
{
	Force force = { N, V2, V3, T, M2, M3 };
	m_FundamentalCombination.push_back(force);
}

void Beam::setForceData
(std::map<std::string, CaseData>* caseMap//[]û��������ʲô��ʽ��
, std::vector<std::string>* factorFC
, std::vector<std::string>* factorNC
, std::vector<std::string>* factorQPC
){
	forceData.setCaseMap(caseMap);
	forceData.setFC(factorFC);
	forceData.setNC(factorNC);
	forceData.setQPC(factorQPC);

}

void Beam::showResult()
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

void Beam::setCheck()
{
	
}