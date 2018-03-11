#include "Material.h"

#include <cmath>
#include <iostream>

#include "xxt.h"

Material::Material(){}

Material::~Material(){}

void Material::setName(double name)
{
	this->name = name;
	updateInfo();
}

double Material::getName() const{ return name; }

double Material::get_E() const{ return E; }

double Material::get_G() const{ return G; }

double Material::get_ν() const{ return ν; }

double Material::get_αc() const{ return αc; }

double Material::get_γ() const{ return γ; }

//*------------------------------------*//

//[]explicit只能用于声明吗？
//explicit Concrete::Concrete(double name)
Concrete::Concrete(double name)
{
	setName(name);
}

Concrete::~Concrete(){}

double Concrete::get_fcuk() const{ return fcuk; }

double Concrete::get_fck() const{ return fck; }

double Concrete::get_ftk() const{ return ftk; }

double Concrete::get_fc() const{ return fc; }

double Concrete::get_ft() const{ return ft; }

double Concrete::get_fcc() const{ return fcc; }

double Concrete::get_fct() const{ return fct; }

double Concrete::get_ε0() const{ return ε0; }

double Concrete::get_εcu() const{ return εcu; }

double Concrete::get_α1() const{ return α1; }

double Concrete::get_β1() const{ return β1; }

void Concrete::updateInfo()
{
	fcuk = name;
	E = 1e5 / (2.2 + 34.7 / fcuk);
	G = 0.4 * E;
	ν = 0.2;
	αc = 1e-5;
	γ = 25;
	fck = 0.88 * calc_αc1() * calc_αc2() * fcuk;
	ftk = 0.88 * 0.395 * pow(fcuk, 0.55) * pow((1 - 1.645 * calc_δ()), 0.45) * calc_αc2();
	fc = fck / 1.4;
	ft = ftk / 1.4;
	fcc = 0.85 * fc;
	fct = 0.55 * ft;
	ε0 = calc_ε0();
	εcu = calc_εcu();
	α1 = calc_α1();
	β1 = calc_β1();
}

double Concrete::calc_αc1() const
{
	//C50及以下0.76，C80取0.82，中间内插
	return interpolation(50, 0.76, 80, 0.82, fcuk, 0);
}

double Concrete::calc_αc2() const
{
	//C40及以下1.00，C80取0.87，中间内插
	return interpolation(40, 1.00, 80, 0.87, fcuk, 0);
}

double Concrete::calc_δ() const
{
	//C40及以下0.12，C80取0.08，中间内插//[]δ取值对于C25及以下的结果与规范对不上
	return interpolation(40, 0.12, 80, 0.08, fcuk, 0);
}

double Concrete::calc_ε0() const
{
	double result = 0.002 + 0.5 * (fcuk - 50) / 100000;
	return result < 0.002 ? 0.002 : result;
}

double Concrete::calc_εcu() const
{
	double result = 0.0033 - (fcuk - 50) / 100000;
	return result > 0.0033 ? 0.0033 : result;
}

double Concrete::calc_α1() const
{
	return interpolation(50, 1.0, 80, 0.94, fcuk, 0);
}

double Concrete::calc_β1() const
{
	return interpolation(50, 0.8, 80, 0.74, fcuk, 0);
}

//*------------------------------------*//

Rebar::Rebar(double name)
{
	setName(name);
}

Rebar::~Rebar(){}

double Rebar::get_fy() const{ return fy; }

double Rebar::get_fy_c() const{ return fy_c; }

double Rebar::get_fyv() const{ return fyv; }

void Rebar::updateInfo()
{
	E = name <= 300 + EPSILON ? 2.1e5 : 2e5;//[XXT]double总存在一些误差，double不要进行精确的比较，如double存放300实际可能为300.00000001，此时不要与300比，要与300.0001比
	G = 0.4 * E;//[]待确定
	ν = 0.3;//[]待确定
	αc = 1.2e-5;//[]待确定
	γ = 78;
	fy = calc_fy();
	fy_c = calc_fy_c();
	fyv = calc_fyv(); 
}

double Rebar::calc_fy() const
{
	return name < 500 - EPSILON ? name / 1.1 : name / 1.15;
}
double Rebar::calc_fy_c() const
{
	return name < 500 - EPSILON ? name / 1.1 : name / 1.2;//[]待确定
}

double Rebar::calc_fyv() const
{
	return fy > 360 ? 360 : fy;
}

//*------------------------------------*//

Steel::Steel(double name)
{
	setName(name);
}

Steel::~Steel(){}

double Steel::get_fy() const{ return fy; }

double Steel::get_f(double t) const{ return calc_f(t); }

double Steel::get_fv(double t) const{ return calc_fv(t); }

double Steel::get_fce(double t) const{ return calc_fce(t); }

void Steel::updateInfo()
{
	fy = name;
	E = 206e3;
	G = 79e3;
	ν = 0.28;//[]待确定
	αc = 1.2e-5;
	γ = 78.5;
}

double Steel::calc_f(double t) const
{
	//Q235
	if (abs(fy - 235) <= EPSILON){
		switch (getThicknessStage(t))
		{
		case 0:
			return 215;
		case 1:
			return 205;
		case 2:
			return 200;
		case 3:
			return 190;
		}
	}
	//Q345
	if (abs(fy - 345) <= EPSILON){
		switch (getThicknessStage(t))
		{
		case 0:
			return 310;
		case 1:
			return 295;
		case 2:
			return 265;
		case 3:
			return 250;
		}
	}
	//Q390
	if (abs(fy - 390) <= EPSILON){
		switch (getThicknessStage(t))
		{
		case 0:
			return 350;
		case 1:
			return 335;
		case 2:
			return 315;
		case 3:
			return 295;
		}
	}
	//Q420
	if (abs(fy - 420) <= EPSILON){
		switch (getThicknessStage(t))
		{
		case 0:
			return 380;
		case 1:
			return 360;
		case 2:
			return 340;
		case 3:
			return 325;
		}
	}
	std::cerr << "未知材料" << std::endl;
}
double Steel::calc_fv(double t) const
{
	return 0;
}

double Steel::calc_fce(double t) const
{
	if (abs(name - 235) <= EPSILON) return 325;
	if (abs(name - 345) <= EPSILON) return 400;
	if (abs(name - 390) <= EPSILON) return 415;
	if (abs(name - 420) <= EPSILON) return 440;
	std::cerr << "未知材料" << std::endl;
	return -999;//给个错值
}

int Steel::getThicknessStage(double t) const
{
	if (abs(fy - 235) <= EPSILON)
	{
		if (t < 16 + EPSILON) return 0;
		if (t < 40 + EPSILON) return 1;
		if (t < 60 + EPSILON) return 2;
		if (t < 100 + EPSILON) return 3;
		return -1;
	}
	if (abs(fy - 345) <= EPSILON || abs(fy - 390) <= EPSILON || abs(fy - 420) <= EPSILON)
	{
		if (t < 16 + EPSILON) return 0;
		if (t < 35 + EPSILON) return 1;
		if (t < 50 + EPSILON) return 2;
		if (t < 100 + EPSILON) return 3;
		return -1;//未知厚度
	}
	return -2;//未知材料
}