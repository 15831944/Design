#include <iostream>
#include <cmath>
#include "design.h"
#include "xxt.h"
#include "concrete_design.h"//�Ź������㹫ʽ

const double AS_SINGLE = 22.5;//������������as=c+AS_SINGLE��10����25�ݽ��
const double AS_DUAL = 47.5;//˫����������as=c+AS_DUAL��10����25�ݽ��
const double ��_LIMIT = 0.01;//�����Ƿ�˫�������������ʽ��ޣ���<��_LIMIT���ţ��ѡݦ�_LIMIT˫��

//[portotype]
///�������
static void calculateAs(double M, double b, double h, double as, double as_c, double fc, double fy, double fy_c, double alpha1, double kxiB, double ��min_c, double& x, double& As, double& As_c);
///����x
static void calculateX(double M, double b, double h0, double fc, double fy, double alpha1, double& x, double& As_c);


Design::Design(){}

Design::~Design(){}

//*------------------------------------*//

DesignBeam::DesignBeam(Beam::Data* data)
{
	this->data = data;
}

DesignBeam::~DesignBeam(){}

void DesignBeam::setData(void* data)
{
	this->data = (Beam::Data*) data;
}

void DesignBeam::design()
{
	prepare();
	designM();
	designV();
}

void DesignBeam::prepare()
{
	void setParameter();
	void setParameterAD();
	void setParameterE();
}

void DesignBeam::designM()
{
	/*
	bool isSingleRowAs = true;//��ʶ�����ֽ��Ƿ񰴵��Ÿֽ����
	bool isSingleRowAs_c = true;//��ʶ��ѹ�ֽ��Ƿ񰴵��Ÿֽ����



	while(true){
		double as = data->c + (isSingleRowAs? AS_SINGLE : AS_DUAL);//as
		double as_c = data->c + (isSingleRowAs_c? AS_SINGLE : AS_DUAL);//as'
		calculateAs(M ,b ,h, as, as_c, fc, fy, fy_c, alpha1, kxiB, x, As, As_c);
		double h0 = data->section - as;

		�� = As / b / h0;//��
		��_c = As_c / b / h0;//��'
		//�ж��Ƿ���Ҫ���¼���
		bool needCal = false;//��ʶ�Ƿ����¼���
		//�ж������ֽ��Ƿ�Ҫ����
		needCal = ((�� < ��_LIMIT) != isSingleRowAs) || needCal;
		if(needCal) isSingleRowAs = !isSingleRowAs;
		//�ж���ѹ�ֽ��Ƿ�Ҫ����
		needCal = ((��_c < ��_LIMIT) != isSingleRowAs_c) || needCal;
		if(needCal) isSingleRowAs_c = !isSingleRowAs_c;
		if(!needCal) break;
	}*/
}

static void calculateAs(double Md, double as, double as_c)
{
	/*double h0 = h - as;
	double sqrtM = h0 * h0 - 2 * M * ForceUnit::k_kN * LengthUnit::k_m / (alpha1 * fc * b);//�������������
	double sqrtLimit = h0 * h0 * (1 - kxiB) * (1 - kxiB);
	if(sqrtM < sqrtLimit){//��ʱx>xb����˫����㣬As'Ҫ�Ȧ�min��
		x = kxiB * h0;
		As_c = (M * ForceUnit::k_kN * LengthUnit::k_m - alpha1 * fc * b * x * (h0 - x / 2)) / fy_c / (h0 - as_c);

		double As_cMin = ��min_c * b * h;
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

void DesignBeam::designV()
{
	
}

void DesignBeam::setParameter()
{
	double ft_Divide_fy = data->concrete->get_ft() / data->longitudinal->get_fy();
	double ft_Divide_fyv = data->concrete->get_ft() / data->stirrup->get_fyv();
	additionData.��b = data->concrete->get_��1() / (1 + data->longitudinal->get_fy() / (data->longitudinal->get_E() * data->concrete->get_��cu()));
	additionData.��min = fmax(0.2, 45 * ft_Divide_fy) / 100; 
	additionData.��max = 0.0275;
	additionData.��min_sv = 0.24 * ft_Divide_fyv;
	if(data->beamType == E_BeamType::E_BT_TRANSFER_BEAM)
	{
		additionData.��min = 0.003;
		additionData.��min_sv = 0.9 * ft_Divide_fyv;
	}
}

void DesignBeam::setParameterAD()
{
	additionData.��Limit = 1;//[]��ȷ�� ���˷��淶����4.6.2
	additionData.��b_AD = 0.5 / additionData.��Limit;//���˷��淶��4.10.3
	additionData.��min_AD = calc_��min_AD();
	additionData.��max_AD = calc_��max_AD();
	additionData.��min_sv_AD = 0.0015;//���˷��淶��4.11.7
}

double DesignBeam::calc_��min_AD()
{//���˷��淶��4.11.7
	if(data->concrete->get_fcuk() < 35 + EPSILON)
	{
		return 0.0025;
	}
	else if(data->concrete->get_fcuk() < 55 + EPSILON)
	{
		return 0.003;
	}
	else
	{
		return 0.0035;
	}
}

double DesignBeam::calc_��max_AD()
{//���˷��淶��4.11.8
	if(data->concrete->get_fcuk() < 25 + EPSILON)
	{
		return data->longitudinal->getName() < 335 + EPSILON ? 0.022 : 0.02;
	}
	else
	{
		return data->longitudinal->getName() < 335 + EPSILON ? 0.025 : 0.024;
	}
}

void DesignBeam::setParameterE()
{
	additionData.��RE_M = 0.75;
	additionData.��RE_V = 0.85;
	double ft_Divide_fy = data->concrete->get_ft() / data->longitudinal->get_fy();
	double ft_Divide_fyv = data->concrete->get_ft() / data->stirrup->get_fyv();
	switch (data->beamType)
	{
	//ת����
	case E_BeamType::E_BT_TRANSFER_BEAM:
		switch (data->Nfb_gz)
		{
		case 0:
			additionData.��b_E = fmin(0.25, additionData.��b);
			additionData.��min_E_LR
				= additionData.��min_E_M
				= 0.006;
			additionData.��min_sv_E = 1.3 * ft_Divide_fyv;
			additionData.��max_E = 0.025;
			additionData.minAs_cRatio_LR = 0.5;
			additionData.minAsRatioContinue = 0.25;
		case 1:
			additionData.��b_E = fmin(0.25, additionData.��b);
			additionData.��min_E_LR
				= additionData.��min_E_M
				= 0.005;
			additionData.��max_E = 0.025;
			additionData.��min_sv_E = 1.2 * ft_Divide_fyv;
			additionData.minAs_cRatio_LR = 0.5;
			additionData.minAsRatioContinue = 0.25;
		case 2:
			additionData.��b_E = fmin(0.35, additionData.��b);
			additionData.��min_E_LR
				= additionData.��min_E_M
				= 0.004;
			additionData.��max_E = 0.025;
			additionData.��min_sv_E = 1.1 * ft_Divide_fyv;
			additionData.minAs_cRatio_LR = 0.3;
			additionData.minAsRatioContinue = 0.25;
		case 3:
		case 4:
			std::cerr << "ת����������ȼ�����Ϊ������" << std::endl;
			break;
		case 5:
			additionData.��min_E_LR
				= additionData.��min_E_M
				= additionData.��min;
			additionData.��max_E = additionData.��max;
			additionData.��min_sv_E = additionData.��min_sv;
			additionData.minAs_cRatio_LR = 0;
			additionData.minAsRatioContinue = 0;
		default:
			std::cerr << "δ֪������ȼ���" << std::endl;
			break;
		}
	//������
	case E_BeamType::E_BT_BEAM:
		if (data->Nfb != 5 || data->Nfb_gz != 5) std::cerr << "�ǿ������������ȼ���" << std::endl;
	default:
		switch (data->Nfb_gz)
		{
		case 0:
			additionData.��b_E = fmin(0.25, additionData.��b);
			additionData.��min_E_LR = fmax(0.4, 80 * ft_Divide_fy) / 100;
			additionData.��min_E_M = fmax(0.3, 65 * ft_Divide_fy) / 100;
			additionData.��max_E = 0.025;
			additionData.��min_sv_E = 0.33 * ft_Divide_fyv;
			additionData.minAs_cRatio_LR = 0.5;
			additionData.minAsRatioContinue = 0.25;
			break;
		case 1:
			additionData.��b_E = fmin(0.25, additionData.��b);
			additionData.��min_E_LR = fmax(0.4, 80 * ft_Divide_fy) / 100;
			additionData.��min_E_M = fmax(0.3, 65 * ft_Divide_fy) / 100;
			additionData.��max_E = 0.025;
			additionData.��min_sv_E = 0.3 * ft_Divide_fyv;
			additionData.minAs_cRatio_LR = 0.5;
			additionData.minAsRatioContinue = 0.25;
			break;
		case 2:
			additionData.��b_E = fmin(0.35, additionData.��b);
			additionData.��min_E_LR = fmax(0.3, 65 * ft_Divide_fy) / 100;
			additionData.��min_E_M = fmax(0.25, 55 * ft_Divide_fy) / 100;
			additionData.��max_E = 0.025;
			additionData.��min_sv_E = 0.28 * ft_Divide_fyv;
			additionData.minAs_cRatio_LR = 0.3;
			additionData.minAsRatioContinue = 0.25;
			break;
		case 3:
			additionData.��b_E = fmin(0.35, additionData.��b);
			additionData.��min_E_LR = fmax(0.25, 55 * ft_Divide_fy) / 100;
			additionData.��min_E_M = fmax(0.2, 45 * ft_Divide_fy) / 100;
			additionData.��max_E = 0.025;
			additionData.��min_sv_E = 0.26 * ft_Divide_fyv;
			additionData.minAs_cRatio_LR = 0.3;
			additionData.minAsRatioContinue = 0;
			break;
		case 4:
			additionData.��b_E = additionData.��b;
			additionData.��min_E_LR = fmax(0.25, 55 * ft_Divide_fy) / 100;
			additionData.��min_E_M = fmax(0.2, 45 * ft_Divide_fy) / 100;
			additionData.��max_E = 0.025;
			additionData.��min_sv_E = 0.26 * ft_Divide_fyv;
			additionData.minAs_cRatio_LR = 0;
			additionData.minAsRatioContinue = 0;
			break;
		case 5:
			additionData.��b_E = additionData.��b;
			additionData.��min_E_LR 
				= additionData.��min_E_M 
				= additionData.��min;
			additionData.��max_E = additionData.��max;
			additionData.��min_sv_E = additionData.��min_sv;
			additionData.minAs_cRatio_LR = 0;
			additionData.minAsRatioContinue = 0;
			break;
		default:
			std::cerr << "δ֪������ȼ���" << std::endl;
			break;
		}
	}
}