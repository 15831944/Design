#pragma once
//作用效应类，存放单工况、组合内力

#include "xxt.h"
#include <iostream>
#include <vector>
#include <set>
#include <map>

struct Force1
{
	double N;
	double V2;
	double V3;
	double T;
	double M2;
	double M3;

	Force1(double N, double V2, double V3, double T, double M2, double M3)
		:N(N), V2(V2), V3(V3), T(T), M2(M2), M3(M3)
	{}

	Force1& operator=(const Force1& f)
	{
		if (this != &f)
		{
			this->N = f.N;
			this->V2 = f.V2;
			this->V3 = f.V3;
			this->T = f.T;
			this->M2 = f.M2;
			this->M3 = f.M3;
		}
		return *this;
	}
	Force1& operator+(const Force1& f)
	{
		this->N += f.N;
		this->V2 += f.V2;
		this->V3 += f.V3;
		this->T += f.T;
		this->M2 += f.M2;
		this->M3 += f.M3;
		return *this;
	}
	Force1& operator-(const Force1& f)
	{
		this->N -= f.N;
		this->V2 -= f.V2;
		this->V3 -= f.V3;
		this->T -= f.T;
		this->M2 -= f.M2;
		this->M3 -= f.M3;
		return *this;
	}
	Force1& operator*(double n)
	{
		this->N *= n;
		this->V2 *= n;
		this->V3 *= n;
		this->T *= n;
		this->M2 *= n;
		this->M3 *= n;
		return *this;
	}
	Force1& operator/(double n)
	{
		if (abs(n - 0) <= EPSILON) std::cerr << "除数为零！" << std::endl;
		this->N /= n;
		this->V2 /= n;
		this->V3 /= n;
		this->T /= n;
		this->M2 /= n;
		this->M3 /= n;
		return *this;
	}
  friend std::ostream& operator<<(std::ostream& os, const Force1& f)//[XXT]加friend为了使这个函数成为Force1的友元函数
  {
    os << "N=" << f.N << std::endl;
    os << "V2=" << f.V2 << std::endl;
    os << "V3=" << f.V3 << std::endl;
    os << "T=" << f.T << std::endl;
    os << "M2=" << f.M2 << std::endl;
    os << "M3=" << f.M3 << std::endl;
    return os;
  }
};
//[XXT]

Force1& operator+(Force1& f1, Force1& f2)
{
  return f1;
}

class ForceEffect
{
public:
	ForceEffect();
	~ForceEffect();
private:
	std::map<std::string, Force1> m_caseMap;//单工况内力
	std::vector<Force1> m_FundamentalCombination;//基本组合
	std::vector<Force1> m_NominalCombination;//标准组合
	std::vector<Force1> m_QuasiPermanentCombination;//准永久组合
};

