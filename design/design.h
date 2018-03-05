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
		//计算参数
		double ξb;//砼极限受压区高度
		double γRE_M;//正截面γRE
		double γRE_V;//斜截面γRE
		//验算参数
		double rhoMin_LR;//支座ρmin
		double rhoMin_c_LR;//支座ρ'min
		double rhoMin_M;//跨中ρmin
		double rhoMin_c_M;//跨中ρ'min
		double rhoMax;//ρmax
		double rhoMin_sv;//ρsv,min
	} additionData;
	Beam::Data* data;
//[]这些函数怎么能写成virtual？
	void prepare();//准备数据
	void setRebarRatio();//设置配筋率限值
	void designM();//设计纵筋
	void designV();//设计箍筋
};
