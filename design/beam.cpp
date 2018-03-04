#include <iostream>

#include "beam.h"
#include "design.h"

Beam::Beam()/*:
data.x(0), As(0), As_c(0), rho(0)
,rho_c(0), Asv(0), rho_sv(0)*/
{ 
	setSection(NULL, 20);
	setMaterial(NULL, NULL, NULL);
	m_FundamentalCombination.resize(6);//[]´ý¸Ä
	m_result.resize(9);
	setCheck();
}

Beam::~Beam(){}

void Beam::setSection(Section* section, double c)
{
	data.section = section;
	data.c = c;
}

void Beam::setMaterial(Concrete* concrete, Rebar* rebarL, Rebar* rebarS)
{
	data.concrete = concrete;
	data.longitudinal = rebarL;
	data.stirrup = rebarS;
}

void Beam::setCalculateParameter(double ¦Ã0, int Nfb, int Nfb_gz)
{
	data.¦Ã0 = ¦Ã0;
	data.Nfb = Nfb;
	data.Nfb_gz = Nfb_gz;
}

void Beam::setForce(double N, double V2, double V3, double T, double M2, double M3)
{
	//´ý¸Ä
	m_FundamentalCombination[0].N = N;
	m_FundamentalCombination[0].V2 = V2;
	m_FundamentalCombination[0].V3 = V3;
	m_FundamentalCombination[0].T = T;
	m_FundamentalCombination[0].M2 = M2;
	m_FundamentalCombination[0].M3 = M3;
}

void Beam::design()
{
	DesignBeam designBeam(&data);
	designBeam.design();
	
	std::cout << "x=" << m_result[0].x << std::endl;
	std::cout << "As=" << m_result[0].As << "¦Ñ=" << m_result[0].rho << std::endl;
	std::cout << "As'=" << m_result[0].As_c << "¦Ñ'=" << m_result[0].rho_c << std::endl;
}

void Beam::setCheck()
{
	
}