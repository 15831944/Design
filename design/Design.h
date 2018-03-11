#pragma once
//设计类，读取构件类进行设计
#include "ConcreteElement.h"

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
	DesignBeam(){}
	explicit DesignBeam(Beam* beam);
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
	} additionData;//相当于数据库
	//一下几行数据为计算的通用数据，根据内力组合的类型取相应值，统一调用designM、designV进行设计
	double ξb;//砼受压区限值
	double γRE;
	double ρmin;//最小配筋率
	double ρmax;//最大配筋率
	double ρmin_sv;//最小配箍率
	//待设计的构件
	Beam* beam;

//[]这些函数怎么能写成virtual？
	void prepare();//准备数据
	void setParameter();//生成非地震组合设计相关参数
	void setParameterAD();//生成人防组合设计相关参数
	void setParameterE();//生成地震组合设计相关参数

	void designULS();//承载能力极限状态设计
	void designSLS();//正常使用极限状态设计
	void designSection//单一截面设计
		(Section* section//要设计的截面
		, BeamSection::E_BeamSectionLocation sectionType//截面位置类型
		, const ForceEffect::CombineForceData& forceData//设计内力
		, BeamSection::ResultFC& resultFC);//存放截面设计结果
	void setDesignParameterN(BeamSection::E_BeamSectionLocation sectionType);//设置非地震组合截面设计参数
	void setDesignParameterAD(BeamSection::E_BeamSectionLocation sectionType);//设置人防组合截面设计参数
	void setDesignParameterE(BeamSection::E_BeamSectionLocation sectionType);//设置地震组合截面设计参数

	void designM(Section* section, const ForceEffect::CombineForceData& forceData, BeamSection::ResultFC& resultFC);//设计纵筋
	void designV(Section* section, const ForceEffect::CombineForceData& forceData, BeamSection::ResultFC& resultFC);//设计箍筋

private://次要内部函数
	double calc_ρmin_AD();//计算ρmin_AD
	double calc_ρmax_AD();//计算ρmax_AD
};
