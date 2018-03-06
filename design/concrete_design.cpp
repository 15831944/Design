/*
���ļ�����Ź�����ʽ
ͳһ�� ���������γߴ硢���ϡ���������������������� ��˳��������
*/
#include "concrete_design.h"
#include <cmath>
#include "design.h"

using namespace std;

const double AS_SINGLE = 22.5;//������������as=c+AS_SINGLE��10����25�ݽ��
const double AS_DUAL = 47.5;//˫����������as=c+AS_DUAL��10����25�ݽ��
const double RHO_LIMIT = 0.01;//�����Ƿ�˫�������������ʽ��ޣ���<RHO_LIMIT���ţ��ѡ�RHO_LIMIT˫��


//[portotype]
///�������
static void calculateAs(double M, double b, double h, double as, double as_c, double fc, double fy, double fy_c, double alpha1, double ksiB, double rhoMin_c, double& x, double& As, double& As_c);
///����x
static void calculateX(double M, double b, double h0, double fc, double fy, double alpha1, double& x, double& As_c);



//[ͨ�ù�ʽ]
void designM(double M
			 , double b, double h, double c
			 , double fc, double fy, double fy_c
			 , double gamaRE, double gama0, double alpha1, double ksiB
			 , double rhoMin_c
			 , double& x, double& As, double& As_c, double& ��, double& ��c){
	double as = c + AS_SINGLE;//as
	double as_c = c + AS_SINGLE;//as'
	double h0 = h - as;
	As_c = rhoMin_c * b * h;
	double Md = M - fy_c * As_c * (h0 - as_c) / E_ForceUnit::E_FU_kN / E_LengthUnit::E_LU_m;
	calculateAs(Md ,b ,h, as, as_c, fc, fy, fy_c, alpha1, ksiB, rhoMin_c, x, As, As_c);
	�� = As / b / h0;//��
	��c = As_c / b / h0;//��'
	if(�� < RHO_LIMIT && ��c < RHO_LIMIT) return;
	if(�� >= RHO_LIMIT) as = c + AS_DUAL;
	if(��c >= RHO_LIMIT) as_c = c + AS_DUAL;
//	As_c = rhoMin_c * b * h;
//	Md = M - fy_c * As_c * (h0 - as_c) / E_ForceUnit::E_FU_kN / E_LengthUnit::E_LU_m;
	calculateAs(Md ,b ,h, as, as_c, fc, fy, fy_c, alpha1, ksiB, rhoMin_c, x, As, As_c);
	��c = As_c / b / h0;//��'
	if(��c < RHO_LIMIT){
		return;
	}else{
		as_c = c + AS_DUAL;
//		As_c = rhoMin_c * b * h;
//		Md = M - fy_c * As_c * (h0 - as_c) / E_ForceUnit::E_FU_kN / E_LengthUnit::E_LU_m;
		calculateAs(Md ,b ,h, as, as_c, fc, fy, fy_c, alpha1, ksiB, rhoMin_c, x, As, As_c);
	}
}

static void calculateAs(double M, double b, double h, double as, double as_c, double fc, double fy, double fy_c, double alpha1, double ksiB, double rhoMin_c, double& x, double& As, double& As_c){
	double h0 = h - as;
	double sqrtM = h0 * h0 - 2 * M * E_ForceUnit::E_FU_kN * E_LengthUnit::E_LU_m / (alpha1 * fc * b);//�������������
	double sqrtLimit = h0 * h0 * (1 - ksiB) * (1 - ksiB);
	if(sqrtM < sqrtLimit){//��ʱx>xb����˫����㣬As'Ҫ�Ȧ�min��
		x = ksiB * h0;
		As_c = (M * E_ForceUnit::E_FU_kN * E_LengthUnit::E_LU_m - alpha1 * fc * b * x * (h0 - x / 2)) / fy_c / (h0 - as_c);
/*		double As_cMin = rhoMin_c * b * h;
		if(As_c < As_cMin){
			As_c = As_cMin;
			double Md = M - fy_c * As_c * (h0 - as_c) / E_ForceUnit::E_FU_kN / E_LengthUnit::E_LU_m;
			sqrtM = h0 * h0 - 2 * Md * E_ForceUnit::E_FU_kN * E_LengthUnit::E_LU_m / (alpha1 * fc * b);
		}*/
	}else{//��ʱx<xb����������㣬As'ȡ��min
		x = h0 - sqrt(sqrtM);
	}
	As = (alpha1 * fc * b * x + fy_c * As_c) / fy;
	if(x < 2 * as_c){
		As = M / fy / (h - as - as_c);
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
void designM(double M, double b, double h, double c, double fc, double fy, double fy_c, double alpha1, double ksiB, double rhoMin_c, double& x, double& As, double& As_c, double& rho, double& ��c){
	bool isSingleRowAs = true;//��ʶ�����ֽ��Ƿ񰴵��Ÿֽ����
	bool isSingleRowAs_c = true;//��ʶ��ѹ�ֽ��Ƿ񰴵��Ÿֽ����
	while(true){
		double as = c + (isSingleRowAs? AS_SINGLE : AS_DUAL);//as
		double as_c = c + (isSingleRowAs_c? AS_SINGLE : AS_DUAL);//as'
		calculateAs(M ,b ,h, as, as_c, fc, fy, fy_c, alpha1, ksiB, x, As, As_c);
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