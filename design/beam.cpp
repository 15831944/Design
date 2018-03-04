#include <cmath>
#include <iostream>

#include "beam.h"
#include "design.h"

Beam::Beam()/*:
data.x(0), As(0), As_c(0), rho(0)
,rho_c(0), Asv(0), rho_sv(0)*/
{ 
	setSection(NULL, 20);
	setMaterial(14.3, 2.01, 360, 360, 360);
	m_F.resize(6);
	result.resize(9);
	setCheck();
}

Beam::~Beam(){}

void Beam::setSection(Section* section, double c){
	data.section = section;
	data.c = c;
}

void Beam::setMaterial(double fc, double ftk, double fy, double fy_c, double fyv){
	data.fc = fc;
	data.ftk = ftk;
	data.fy = fy;
	data.fy_c = fy_c;
	data.fyv = fyv;
}

void Beam::setCalculateParameter(double gama0, double gamaRE, int Nfb, int Nfb_gz){
	data.gama0 = gama0;
	data.gamaRE = gamaRE;
	data.Nfb = Nfb;
	data.Nfb_gz = Nfb_gz;
}

void Beam::setForce(double n, double f2, double f3, double t, double m2, double m3){
	m_F[0] = n;
	m_F[1] = f2;
	m_F[2] = f3;
	m_F[3] = t;
	m_F[4] = m2;
	m_F[5] = m3;
}

void Beam::design(){
	DesignBeam designBeam(&data);
	designBeam.design();
	
	std::cout << "x=" << result[0].x << std::endl;
	std::cout << "As=" << result[0].As  << "¦Ñ=" << result[0].rho << std::endl;
	std::cout << "As'=" << result[0].As_c  << "¦Ñ'=" << result[0].rho_c << std::endl;
}

void Beam::setCheck(){
	data.rhoMin = 0.002;
	data.rhoMax = 0.025;
	data.rhoMin_c = 0.0025;
	data.rhoMin_sv = 0.002;
}