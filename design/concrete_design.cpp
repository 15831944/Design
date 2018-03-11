/*
���ļ�����Ź�����ʽ
ͳһ�� ���������γߴ硢���ϡ���������������������� ��˳��������
*/
#include <cmath>

#include "concrete_design.h"
#include "xxt.h"


const double AS_SINGLE = 22.5;//������������as=c+AS_SINGLE��10����25�ݽ��
const double AS_DUAL = 47.5;//˫����������as=c+AS_DUAL��10����25�ݽ��
const double ��_LIMIT = 0.01;//�����Ƿ�˫�������������ʽ��ޣ���<RHO_LIMIT���ţ��ѡ�RHO_LIMIT˫��


//[portotype]
///����x
double calc_x(double Md, double b, double h0, double fc, double ��1, double ��b);
///�������
static void calculateAs(double M, double b, double h, double as, double as_c, double fc, double fy, double fy_c, double ��1, double ��b, double rhoMin_c, double& x, double& As, double& As_c);


//[ͨ�ù�ʽ]
void designM_Rect(double M
			 , double b, double h, double c
			 , double fc, double fy, double fy_c
			 , double ��RE, double ��0, double ��1, double ��b
			 , double& x, double& As, double& As_c, double& ��, double& ��c){
	bool isSingleRowAs = true;//��ʶ�����ֽ��Ƿ񰴵��Ÿֽ����
	bool isSingleRowAs_c = true;//��ʶ��ѹ�ֽ��Ƿ񰴵��Ÿֽ����
	while (true)
	{
		double as = c + (isSingleRowAs ? AS_SINGLE : AS_DUAL);//as
		double as_c = c + (isSingleRowAs_c ? AS_SINGLE : AS_DUAL);//as'
		double h0 = h - as;
		double Md = ��0 * ��RE * M;
		x = calc_x(Md, b, h0, fc, ��1, ��b);
		As_c = (��0 * ��RE * M * E_ForceUnit::E_FU_kN * E_LengthUnit::E_LU_m - ��1 * fc * b * x * (h0 - x / 2)) / (fy_c * (h0 - as_c));
		if (As_c > EPSILON && x <= 2 * as_c)
		{//���Ź桷6.2.14
			As = ��0 * ��RE * M * E_ForceUnit::E_FU_kN * E_LengthUnit::E_LU_m / (fy_c * (h0 - as_c));
		}
		else
		{
			As = (��1 * fc * b * x + fy_c * As_c) / fy;
		}
		�� = As / b / h0;//��
		��c = As_c / b / h0;//��'
		//�ж��Ƿ���Ҫ���¼���
		bool needCal = false;//��ʶ�Ƿ����¼���
		//�ж������ֽ��Ƿ�Ҫ����
		if ((�� < ��_LIMIT) != isSingleRowAs)
		{
			needCal = true;
			isSingleRowAs = !isSingleRowAs;
		}
		//�ж���ѹ�ֽ��Ƿ�Ҫ����
		if ((��c < ��_LIMIT) != isSingleRowAs_c)
		{
			needCal = true;
			isSingleRowAs_c = !isSingleRowAs_c;
		}
		if (!needCal) break;
	}
}

double calc_x(double Md, double b, double h0, double fc, double ��1, double ��b)
{
	double sqrtM = h0 * h0 - 2 * Md * E_ForceUnit::E_FU_kN * E_LengthUnit::E_LU_m / (��1 * fc * b);//�������������
	double sqrtLimit = h0 * h0 * (1 - ��b) * (1 - ��b);
	if (sqrtM < sqrtLimit)
	{//��ʱx>xb����˫����㣬As'Ҫ�Ȧ�min��
		return ��b * h0;
	}
	else
	{
		return h0 - sqrt(sqrtM);
	}
}


/*
void beam_CalculateTransverseRebar(double v, double b, double h0, double ft, double fyv, double alphaCV, double s, double& asv){
	double vDesign = v * UNIT_TRANSFER_FACTOR_KN_TO_N -alphaCV * ft * b * h0;
	asv = vDesign * s / fyv / h0;
}
*/
//[��]

//[��]

//[ǽ]























/*
void designM(double M, double b, double h, double c, double fc, double fy, double fy_c, double ��1, double ��b, double rhoMin_c, double& x, double& As, double& As_c, double& rho, double& ��c){
	bool isSingleRowAs = true;//��ʶ�����ֽ��Ƿ񰴵��Ÿֽ����
	bool isSingleRowAs_c = true;//��ʶ��ѹ�ֽ��Ƿ񰴵��Ÿֽ����
	while(true){
		double as = c + (isSingleRowAs? AS_SINGLE : AS_DUAL);//as
		double as_c = c + (isSingleRowAs_c? AS_SINGLE : AS_DUAL);//as'
		calculateAs(M ,b ,h, as, as_c, fc, fy, fy_c, ��1, ��b, x, As, As_c);
		double h0 = h - as;
		�� = As / b / h0;//��
		��c = As_c / b / h0;//��'
		//�ж��Ƿ���Ҫ���¼���
		bool needCal = false;//��ʶ�Ƿ����¼���
		//�ж������ֽ��Ƿ�Ҫ����
		needCal = ((�� < RHO_LIMIT) != isSingleRowAs) || needCal;
		if(needCal) isSingleRowAs = !isSingleRowAs;
		//�ж���ѹ�ֽ��Ƿ�Ҫ����
		needCal = ((��c < RHO_LIMIT) != isSingleRowAs_c) || needCal;
		if(needCal) isSingleRowAs_c = !isSingleRowAs_c;
		if(!needCal) break;
	}
}*/