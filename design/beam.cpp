#include <iostream>

#include "beam.h"
#include "design.h"

Beam::Beam()/*:
data.x(0), As(0), As_c(0), rho(0)
,rho_c(0), Asv(0), rho_sv(0)*/
{ 
	setSection(NULL, 20);
	setMaterial(NULL, NULL, NULL, NULL);
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

void Beam::setCalculateParameter(double gama0, double gamaRE, int Nfb, int Nfb_gz){
	data.gama0 = gama0;
	data.gamaRE = gamaRE;
	data.Nfb = Nfb;
	data.Nfb_gz = Nfb_gz;
}

void Beam::setForce(double n, double f2, double f3, double t, double m2, double m3){
	
}

void Beam::design(){
	DesignBeam designBeam(&data);
	designBeam.design();
	
	std::cout << "x=" << m_result[0].x << std::endl;
	std::cout << "As=" << m_result[0].As << "¦Ñ=" << m_result[0].rho << std::endl;
	std::cout << "As'=" << m_result[0].As_c << "¦Ñ'=" << m_result[0].rho_c << std::endl;
}

void Beam::setCheck(){
	data.rhoMin = 0.002;
	data.rhoMax = 0.025;
	data.rhoMin_c = 0.0025;
	data.rhoMin_sv = 0.002;
}