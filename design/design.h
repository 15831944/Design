#pragma once

#include "beam.h"

//����λת��ͳһ��N
enum E_ForceUnit
{
	E_FU_N = 1,
	E_FU_kN = 1000
};
//���ȵ�λת��ͳһ��mm
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

	virtual void setData(void* data) = 0;//��������
	virtual void design() = 0;//�������
protected:

};

//*------------------------------------*//

class DesignBeam : public Design
{
public:
	explicit DesignBeam(Beam::Data* data);
	virtual ~DesignBeam();
	virtual void setData(void* data);//��������
	virtual void design();//�������additionData
private:
	struct AdditionData
	{//��������
		//�������
		double ��b;//�ż�����ѹ���߶�
		double ��RE_M;//�������RE
		double ��RE_V;//б�����RE
		//�������
		double rhoMin_LR;//֧����min
		double rhoMin_c_LR;//֧����'min
		double rhoMin_M;//���Ц�min
		double rhoMin_c_M;//���Ц�'min
		double rhoMax;//��max
		double rhoMin_sv;//��sv,min
	} additionData;
	Beam::Data* data;
//[]��Щ������ô��д��virtual��
	void prepare();//׼������
	void setRebarRatio();//�����������ֵ
	void designM();//����ݽ�
	void designV();//��ƹ���
};
