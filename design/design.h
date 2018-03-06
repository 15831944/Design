#pragma once

#include "beam.h"

//力单位转换统一到N
enum E_ForceUnit
{
	E_FU_N = 1,
	E_FU_kN = 1000
};
//长度单位转换统一到mm
enum E_LengthUnit
{
	E_LU_mm = 1,//mm
	E_LU_m = 1000//m
};

class Design
{
public:
	Design();
	virtual ~Design();

	virtual void setData(void* data) = 0;//塞入数据
	virtual void design() = 0;//进行设计
protected:

};

//*------------------------------------*//

class DesignBeam : public Design
{
public:
	explicit DesignBeam(Beam::Data* data);
	virtual ~DesignBeam();
	virtual void setData(void* data);//塞入数据
	virtual void design();//进行设计additionData
private:
	struct AdditionData
	{//附加数据
		//非地震组合设计
		double ξb;//砼受压区限值
		double ρmin;//最小配筋率
		double ρmax;//最大配筋率
		double ρmin_sv;//最小配箍率
		//人防组合设计
		double βLimit;//延性限值
		double ξb_AD;
		double ρmin_AD;
		double ρmax_AD;
		double ρmin_sv_AD;
		//地震组合设计
		double ξb_E;
		double γRE_M;//正截面γRE
		double γRE_V;//斜截面γRE
		double ρmin_E_LR;//支座ρmin
		double ρmin_E_M;//跨中ρmin
		double ρmax_E;
		double ρmin_sv_E;
		double minAs_cRatio_LR;//梁端下铁与上铁的最小比值
		double minAsRatioContinue;//贯通钢筋与最大配筋的比值
	} additionData;
	Beam::Data* data;
//[]这些函数怎么能写成virtual？
	void prepare();//准备数据
	void setParameter();//生成非地震组合设计相关参数
	void setParameterAD();//生成人防组合设计相关参数
	void setParameterE();//生成地震组合设计相关参数
	void designM();//设计纵筋
	void designV();//设计箍筋

	double calc_ρmin_AD();//计算ρmin_AD
	double calc_ρmax_AD();//计算ρmax_AD
};
