#pragma once

#include "beam.h"

//����λת��ͳһ��N
enum ForceUnit{
	k_N=1,
	k_kN=1000
};
//���ȵ�λת��ͳһ��mm
enum LengthUnit{
	k_mm=1,//mm
	k_m=1000//m
};

class Design{
public:
	Design();
	virtual ~Design();

	virtual void setData(void* data) = 0;//��������
	virtual void design() = 0;//�������
protected:

};

//*------------------------------------*//

class DesignBeam : public Design{
public:
	DesignBeam(Beam::Data* data);
	virtual ~DesignBeam();
	virtual void setData(void* data);//��������
	virtual void design();//�������additionData
private:
	struct AdditionData{//��������
		double ksiB;
	} additionData;
	Beam::Data* data;

	void prepare();//׼������
	void designM();//����ݽ�
	void designV();//��ƹ���
};
