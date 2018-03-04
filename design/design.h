#pragma once

#include "beam.h"

//力单位转换统一到N
enum ForceUnit{
	k_N=1,
	k_kN=1000
};
//长度单位转换统一到mm
enum LengthUnit{
	k_mm=1,//mm
	k_m=1000//m
};

class Design{
public:
	Design();
	virtual ~Design();

	virtual void setData(void* data) = 0;//塞入数据
	virtual void design() = 0;//进行设计
protected:

};

//*------------------------------------*//

class DesignBeam : public Design{
public:
	DesignBeam(Beam::Data* data);
	virtual ~DesignBeam();
	virtual void setData(void* data);//塞入数据
	virtual void design();//进行设计additionData
private:
	struct AdditionData{//附加数据
		double ksiB;
	} additionData;
	Beam::Data* data;

	void prepare();//准备数据
	void designM();//设计纵筋
	void designV();//设计箍筋
};
