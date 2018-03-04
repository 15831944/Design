#ifndef _beam_h
#define _beam_h

#include <vector>
#include "section.h"

class Beam{
	//static double gama0;
public:
	Beam();
	~Beam();

	void setSection(Section* section, double c);//设置几何参数
	void setMaterial(double fc, double ftk, double fy, double fy_c, double fyv);//设置材料强度
	void setCalculateParameter(double gama0, double gamaRE, int Nfb, int Nfb_gz);//设置计算参数
	void setForce(double n, double f2, double f3, double t, double m2, double m3);//设置内力
	void design();//配筋设计
	void check();//验算配筋结果

private:
	struct Data{
		//[?]怎么给struct赋初值？
		//几何参数
		Section* section;
		double c;//保护层厚度
		//材料参数
		//MaterialC concrete;
		double fc;
		double ftk;
		//MaterialS steelBar;
		double fy;
		double fy_c;
		double fyv;
		//计算参数
		double gama0;//γ0
		double gamaRE;//γRE
		int Nfb;//抗震等级
		int Nfb_gz;//抗震构造等级
		//验算参数
		double rhoMin;//ρmin
		double rhoMin_c;//ρmin
		double rhoMax;//ρmax
		double rhoMin_sv;//ρsv,min
	} data;
	struct Result{//设计结果
	double x;//受压区高度mm
		double As;//受拉钢筋面积mm²
		double As_c;//受压钢筋面积mm²
		double rho;//受拉钢筋配筋率ρ
		double rho_c;//受压钢筋配筋率ρ'
		double Asv;//箍筋面积mm²
		double rho_sv;//箍筋面积率ρsv
	};
	std::vector<Result> result;//各断面设计结果
	std::vector<double> m_F;//内力N/V2/V3/T/M2/M3，kN，m
	
	void setCheck();

	friend class DesignBeam;
};

#endif