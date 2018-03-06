#pragma once

#include <vector>
#include <map>
#include "section.h"
#include "material.h"
#include "xxt.h"

enum E_BeamType
{
	E_BT_FRAME_BEAM = 0,//框架梁
	E_BT_BEAM,//非框架梁
	E_BT_COUPLING_BEAM,//连梁
	E_BT_TRANSFER_BEAM//转换梁
};

class Beam
{
	//static double gama0;
public:
	Beam();
	~Beam();

	void setSection(Section* section, double c);//设置几何参数
	void setMaterial(Concrete* concrete, Rebar* rebarL, Rebar* rebarS, Steel* steel);//设置材料
	void setCalculateParameter(double γ0, int Nfb, int Nfb_gz);//设置计算参数
	void setBeamType(E_BeamType beamType);//设置梁类型
	void setForce(double N, double V2, double V3, double T, double M2, double M3);//设置内力
	void showResult();//展示结果

private:
	struct Data
	{//基本参数
		//[?]怎么给struct赋初值？
		//几何参数
		Section* section;//截面参数
		double c;//保护层厚度
		//材料参数
		Concrete* concrete;//砼材料
		Rebar* longitudinal;//纵筋材料
		Rebar* stirrup;//箍筋材料
		Steel* skeleton;//钢骨材料
		//计算参数
		E_BeamType beamType;//梁类型
		double γ0;//γ0
		int Nfb;//抗震等级，0-特一级/1-一级/2-二级/3-三级/4-四级/5-非抗震
		int Nfb_gz;//抗震构造等级，0-特一级/1-一级/2-二级/3-三级/4-四级/5-非抗震
	} data;
	std::map<std::string, Force> m_caseMap;//单工况内力
	std::vector<Force> m_FundamentalCombination;//基本组合
	std::vector<Force> m_NominalCombination;//标准组合
	std::vector<Force> m_QuasiPermanentCombination;//准永久组合
	struct Result
	{//设计结果
		double x;//受压区高度mm
		double As;//受拉钢筋面积mm²
		double As_c;//受压钢筋面积mm²
		double ρ;//受拉钢筋配筋率ρ
		double ρc;//受压钢筋配筋率ρ'
		double Asv;//箍筋面积mm²
		double ρsv;//箍筋面积率ρsv
	};
	std::vector<Result> m_result;//各断面设计结果
	
	void setCheck();

	friend class DesignBeam;
};