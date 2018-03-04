#include "design.h"
#include "concrete_design.h"//�Ź������㹫ʽ

const double AS_SINGLE = 22.5;//������������as=c+AS_SINGLE��10����25�ݽ��
const double AS_DUAL = 47.5;//˫����������as=c+AS_DUAL��10����25�ݽ��
const double RHO_LIMIT = 0.01;//�����Ƿ�˫�������������ʽ��ޣ���<RHO_LIMIT���ţ��ѡ�RHO_LIMIT˫��

//[portotype]
///�����b
double getKxiB(double beta1, double fy, double Es, double stressLimitC);

///�������
static void calculateAs(double M, double b, double h, double as, double as_c, double fc, double fy, double fy_c, double alpha1, double ksiB, double rhoMin_c, double& x, double& As, double& As_c);
///����x
static void calculateX(double M, double b, double h0, double fc, double fy, double alpha1, double& x, double& As_c);


Design::Design(){}

Design::~Design(){}

//*------------------------------------*//

DesignBeam::DesignBeam(Beam::Data* data){
	this->data = data;
}

DesignBeam::~DesignBeam(){}

void DesignBeam::setData(void* data){
	this->data = (Beam::Data*) data;
}

void DesignBeam::design(){
	prepare();
	designM();
	designV();
}

void DesignBeam::prepare(){
	//additionData.ksiB = getKxiB(additionData.beta1, data->fy, 200000, additionData.stressLimitC);//Es����cu����
}

void DesignBeam::designM(){
	/*
	bool isSingleRowAs = true;//��ʶ�����ֽ��Ƿ񰴵��Ÿֽ����
	bool isSingleRowAs_c = true;//��ʶ��ѹ�ֽ��Ƿ񰴵��Ÿֽ����



	while(true){
		double as = data->c + (isSingleRowAs? AS_SINGLE : AS_DUAL);//as
		double as_c = data->c + (isSingleRowAs_c? AS_SINGLE : AS_DUAL);//as'
		calculateAs(M ,b ,h, as, as_c, fc, fy, fy_c, alpha1, ksiB, x, As, As_c);
		double h0 = data->section - as;

		rho = As / b / h0;//��
		rho_c = As_c / b / h0;//��'
		//�ж��Ƿ���Ҫ���¼���
		bool needCal = false;//��ʶ�Ƿ����¼���
		//�ж������ֽ��Ƿ�Ҫ����
		needCal = ((rho < RHO_LIMIT) != isSingleRowAs) || needCal;
		if(needCal) isSingleRowAs = !isSingleRowAs;
		//�ж���ѹ�ֽ��Ƿ�Ҫ����
		needCal = ((rho_c < RHO_LIMIT) != isSingleRowAs_c) || needCal;
		if(needCal) isSingleRowAs_c = !isSingleRowAs_c;
		if(!needCal) break;
	}*/
}

static void calculateAs(double Md, double as, double as_c){
	/*double h0 = h - as;
	double sqrtM = h0 * h0 - 2 * M * ForceUnit::k_kN * LengthUnit::k_m / (alpha1 * fc * b);//�������������
	double sqrtLimit = h0 * h0 * (1 - ksiB) * (1 - ksiB);
	if(sqrtM < sqrtLimit){//��ʱx>xb����˫����㣬As'Ҫ�Ȧ�min��
		x = ksiB * h0;
		As_c = (M * ForceUnit::k_kN * LengthUnit::k_m - alpha1 * fc * b * x * (h0 - x / 2)) / fy_c / (h0 - as_c);

		double As_cMin = rhoMin_c * b * h;
		if(As_c < As_cMin){
			As_c = As_cMin;
			double Md = M - fy_c * As_c * (h0 - as_c) / ForceUnit::k_kN / LengthUnit::k_m;
			sqrtM = h0 * h0 - 2 * Md * ForceUnit::k_kN * LengthUnit::k_m / (alpha1 * fc * b);
		}

	}else{//��ʱx<xb����������㣬As'ȡ��min
		x = h0 - sqrt(sqrtM);
	}
	As = (alpha1 * fc * b * x + fy_c * As_c) / fy;
	if(x < 2 * as_c){
		As = M / fy / (h - as - as_c);
	}*/
}

void DesignBeam::designV(){
	
}

double getKxiB(double beta1, double fy, double Es, double stressLimitC){
	return beta1 / (1 + fy / (Es * stressLimitC));
}