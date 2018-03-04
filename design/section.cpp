#include "Section.h"

//using namespace std;

Section::Section() {}

Section::~Section() {}

void Section::set_angle(double angle)
{
	this->m_angle = angle;
}

double Section::get_angle() const{return m_angle;}

RectSection::RectSection(double b, double h)
:m_b(b)
,m_h(h)
{}

RectSection::~RectSection(){}

void RectSection::set_b(double b)
{
	this->m_b = b;
}

void RectSection::set_h(double h)
{
	this->m_h = h;
}

double RectSection::get_b() const{return m_b;}

double RectSection::get_h() const{return m_h;}

CircleSection::CircleSection(){}

CircleSection::~CircleSection(){}

void CircleSection::set_d(double d)
{
	this->m_d = d;
}

double CircleSection::get_d() const{return m_d;}