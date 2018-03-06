#include <iostream>

#include "beam.h"

Beam::Beam()
{ 
	setSection(NULL, 20);
	setMaterial(NULL, NULL, NULL, NULL);
	setCalculateParameter(0, 5, 5);
	setBeamType(E_BeamType::E_BT_BEAM);
	m_FundamentalCombination.resize(6);//[]����
	m_result.resize(9);
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
	//����
	m_FundamentalCombination[0].N = N;
	m_FundamentalCombination[0].V2 = V2;
	m_FundamentalCombination[0].V3 = V3;
	m_FundamentalCombination[0].T = T;
	m_FundamentalCombination[0].M2 = M2;
	m_FundamentalCombination[0].M3 = M3;
}

void Beam::showResult()
{
	std::cout << "������" << std::endl;
	std::cout << "x=" << m_result[0].x << std::endl;
	std::cout << "As=" << m_result[0].As << "  ��=" << m_result[0].�� << std::endl;
	std::cout << "As'=" << m_result[0].As_c << "  ��'=" << m_result[0].��c << std::endl;
	std::cout << "б����" << std::endl;
	std::cout << "Asv=" << m_result[0].Asv << "  ��sv=" << m_result[0].��sv << std::endl;
}

void Beam::setCheck()
{
	
}