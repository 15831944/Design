#include <iostream>

#include "beam.h"

Beam::Beam()
{ 
	setSection(NULL, 20);
	setMaterial(NULL, NULL, NULL, NULL);
	setCalculateParameter(0, 5, 5);
	setBeamType(E_BeamType::E_BT_BEAM);
	m_FundamentalCombination.resize(6);//[]待改
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

void Beam::setCalculateParameter(double γ0, int Nfb, int Nfb_gz)
{
	data.γ0 = γ0;
	data.Nfb = Nfb;
	data.Nfb_gz = Nfb_gz;
}

void Beam::setBeamType(E_BeamType beamType)
{
	data.beamType = beamType;
}

void Beam::setForce(double N, double V2, double V3, double T, double M2, double M3)
{
	//待改
	m_FundamentalCombination[0].N = N;
	m_FundamentalCombination[0].V2 = V2;
	m_FundamentalCombination[0].V3 = V3;
	m_FundamentalCombination[0].T = T;
	m_FundamentalCombination[0].M2 = M2;
	m_FundamentalCombination[0].M3 = M3;
}

void Beam::showResult()
{
	std::cout << "正截面" << std::endl;
	std::cout << "x=" << m_result[0].x << std::endl;
	std::cout << "As=" << m_result[0].As << "  ρ=" << m_result[0].ρ << std::endl;
	std::cout << "As'=" << m_result[0].As_c << "  ρ'=" << m_result[0].ρc << std::endl;
	std::cout << "斜截面" << std::endl;
	std::cout << "Asv=" << m_result[0].Asv << "  ρsv=" << m_result[0].ρsv << std::endl;
}

void Beam::setCheck()
{
	
}