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
		//�ǵ���������
		double ��b;//����ѹ����ֵ
		double ��min;//��С�����
		double ��max;//��������
		double ��min_sv;//��С�乿��
		//�˷�������
		double ��Limit;//������ֵ
		double ��b_AD;
		double ��min_AD;
		double ��max_AD;
		double ��min_sv_AD;
		//����������
		double ��b_E;
		double ��RE_M;//�������RE
		double ��RE_V;//б�����RE
		double ��min_E_LR;//֧����min
		double ��min_E_M;//���Ц�min
		double ��max_E;
		double ��min_sv_E;
		double minAs_cRatio_LR;//������������������С��ֵ
		double minAsRatioContinue;//��ͨ�ֽ���������ı�ֵ
	} additionData;
	Beam::Data* data;
//[]��Щ������ô��д��virtual��
	void prepare();//׼������
	void setParameter();//���ɷǵ�����������ز���
	void setParameterAD();//�����˷���������ز���
	void setParameterE();//���ɵ�����������ز���
	void designM();//����ݽ�
	void designV();//��ƹ���

	double calc_��min_AD();//�����min_AD
	double calc_��max_AD();//�����max_AD
};
