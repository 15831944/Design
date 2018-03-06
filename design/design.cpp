#include <iostream>
#include <cmath>
#include "design.h"
#include "xxt.h"
#include "concrete_design.h"//砼构件计算公式

const double AS_SINGLE = 22.5;//单筋保护层额外厚度as=c+AS_SINGLE按10箍筋25纵筋考虑
const double AS_DUAL = 47.5;//双筋保护层额外厚度as=c+AS_DUAL按10箍筋25纵筋考虑
const double ρ_LIMIT = 0.01;//控制是否按双排配筋计算的配筋率界限，ρ<ρ_LIMIT单排，ρ≥ρ_LIMIT双排

//[portotype]
///计算配筋
static void calculateAs(double M, double b, double h, double as, double as_c, double fc, double fy, double fy_c, double alpha1, double kxiB, double ρmin_c, double& x, double& As, double& As_c);
///计算x
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
	bool isSingleRowAs = true;//标识受拉钢筋是否按单排钢筋计算
	bool isSingleRowAs_c = true;//标识受压钢筋是否按单排钢筋计算



	while(true){
		double as = data->c + (isSingleRowAs? AS_SINGLE : AS_DUAL);//as
		double as_c = data->c + (isSingleRowAs_c? AS_SINGLE : AS_DUAL);//as'
		calculateAs(M ,b ,h, as, as_c, fc, fy, fy_c, alpha1, kxiB, x, As, As_c);
		double h0 = data->section - as;

		ρ = As / b / h0;//ρ
		ρ_c = As_c / b / h0;//ρ'
		//判断是否需要重新计算
		bool needCal = false;//标识是否重新计算
		//判断受拉钢筋是否要重算
		needCal = ((ρ < ρ_LIMIT) != isSingleRowAs) || needCal;
		if(needCal) isSingleRowAs = !isSingleRowAs;
		//判断受压钢筋是否要重算
		needCal = ((ρ_c < ρ_LIMIT) != isSingleRowAs_c) || needCal;
		if(needCal) isSingleRowAs_c = !isSingleRowAs_c;
		if(!needCal) break;
	}*/
}

static void calculateAs(double Md, double as, double as_c)
{
	/*double h0 = h - as;
	double sqrtM = h0 * h0 - 2 * M * ForceUnit::k_kN * LengthUnit::k_m / (alpha1 * fc * b);//根号里面的内容
	double sqrtLimit = h0 * h0 * (1 - kxiB) * (1 - kxiB);
	if(sqrtM < sqrtLimit){//此时x>xb，按双筋计算，As'要比ρmin大
		x = kxiB * h0;
		As_c = (M * ForceUnit::k_kN * LengthUnit::k_m - alpha1 * fc * b * x * (h0 - x / 2)) / fy_c / (h0 - as_c);

		double As_cMin = ρmin_c * b * h;
		if(As_c < As_cMin){
			As_c = As_cMin;
			double Md = M - fy_c * As_c * (h0 - as_c) / ForceUnit::k_kN / LengthUnit::k_m;
			sqrtM = h0 * h0 - 2 * Md * ForceUnit::k_kN * LengthUnit::k_m / (alpha1 * fc * b);
		}

	}else{//此时x<xb，按单筋计算，As'取ρmin
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
	additionData.ξb = data->concrete->get_β1() / (1 + data->longitudinal->get_fy() / (data->longitudinal->get_E() * data->concrete->get_εcu()));
	additionData.ρmin = fmax(0.2, 45 * ft_Divide_fy) / 100; 
	additionData.ρmax = 0.0275;
	additionData.ρmin_sv = 0.24 * ft_Divide_fyv;
	if(data->beamType == E_BeamType::E_BT_TRANSFER_BEAM)
	{
		additionData.ρmin = 0.003;
		additionData.ρmin_sv = 0.9 * ft_Divide_fyv;
	}
}

void DesignBeam::setParameterAD()
{
	additionData.βLimit = 1;//[]待确定 《人防规范》表4.6.2
	additionData.ξb_AD = 0.5 / additionData.βLimit;//《人防规范》4.10.3
	additionData.ρmin_AD = calc_ρmin_AD();
	additionData.ρmax_AD = calc_ρmax_AD();
	additionData.ρmin_sv_AD = 0.0015;//《人防规范》4.11.7
}

double DesignBeam::calc_ρmin_AD()
{//《人防规范》4.11.7
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

double DesignBeam::calc_ρmax_AD()
{//《人防规范》4.11.8
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
	additionData.γRE_M = 0.75;
	additionData.γRE_V = 0.85;
	double ft_Divide_fy = data->concrete->get_ft() / data->longitudinal->get_fy();
	double ft_Divide_fyv = data->concrete->get_ft() / data->stirrup->get_fyv();
	switch (data->beamType)
	{
	//转换梁
	case E_BeamType::E_BT_TRANSFER_BEAM:
		switch (data->Nfb_gz)
		{
		case 0:
			additionData.ξb_E = fmin(0.25, additionData.ξb);
			additionData.ρmin_E_LR
				= additionData.ρmin_E_M
				= 0.006;
			additionData.ρmin_sv_E = 1.3 * ft_Divide_fyv;
			additionData.ρmax_E = 0.025;
			additionData.minAs_cRatio_LR = 0.5;
			additionData.minAsRatioContinue = 0.25;
		case 1:
			additionData.ξb_E = fmin(0.25, additionData.ξb);
			additionData.ρmin_E_LR
				= additionData.ρmin_E_M
				= 0.005;
			additionData.ρmax_E = 0.025;
			additionData.ρmin_sv_E = 1.2 * ft_Divide_fyv;
			additionData.minAs_cRatio_LR = 0.5;
			additionData.minAsRatioContinue = 0.25;
		case 2:
			additionData.ξb_E = fmin(0.35, additionData.ξb);
			additionData.ρmin_E_LR
				= additionData.ρmin_E_M
				= 0.004;
			additionData.ρmax_E = 0.025;
			additionData.ρmin_sv_E = 1.1 * ft_Divide_fyv;
			additionData.minAs_cRatio_LR = 0.3;
			additionData.minAsRatioContinue = 0.25;
		case 3:
		case 4:
			std::cerr << "转换梁抗震构造等级至少为二级！" << std::endl;
			break;
		case 5:
			additionData.ρmin_E_LR
				= additionData.ρmin_E_M
				= additionData.ρmin;
			additionData.ρmax_E = additionData.ρmax;
			additionData.ρmin_sv_E = additionData.ρmin_sv;
			additionData.minAs_cRatio_LR = 0;
			additionData.minAsRatioContinue = 0;
		default:
			std::cerr << "未知抗震构造等级！" << std::endl;
			break;
		}
	//其他梁
	case E_BeamType::E_BT_BEAM:
		if (data->Nfb != 5 || data->Nfb_gz != 5) std::cerr << "非框架梁包含抗震等级！" << std::endl;
	default:
		switch (data->Nfb_gz)
		{
		case 0:
			additionData.ξb_E = fmin(0.25, additionData.ξb);
			additionData.ρmin_E_LR = fmax(0.4, 80 * ft_Divide_fy) / 100;
			additionData.ρmin_E_M = fmax(0.3, 65 * ft_Divide_fy) / 100;
			additionData.ρmax_E = 0.025;
			additionData.ρmin_sv_E = 0.33 * ft_Divide_fyv;
			additionData.minAs_cRatio_LR = 0.5;
			additionData.minAsRatioContinue = 0.25;
			break;
		case 1:
			additionData.ξb_E = fmin(0.25, additionData.ξb);
			additionData.ρmin_E_LR = fmax(0.4, 80 * ft_Divide_fy) / 100;
			additionData.ρmin_E_M = fmax(0.3, 65 * ft_Divide_fy) / 100;
			additionData.ρmax_E = 0.025;
			additionData.ρmin_sv_E = 0.3 * ft_Divide_fyv;
			additionData.minAs_cRatio_LR = 0.5;
			additionData.minAsRatioContinue = 0.25;
			break;
		case 2:
			additionData.ξb_E = fmin(0.35, additionData.ξb);
			additionData.ρmin_E_LR = fmax(0.3, 65 * ft_Divide_fy) / 100;
			additionData.ρmin_E_M = fmax(0.25, 55 * ft_Divide_fy) / 100;
			additionData.ρmax_E = 0.025;
			additionData.ρmin_sv_E = 0.28 * ft_Divide_fyv;
			additionData.minAs_cRatio_LR = 0.3;
			additionData.minAsRatioContinue = 0.25;
			break;
		case 3:
			additionData.ξb_E = fmin(0.35, additionData.ξb);
			additionData.ρmin_E_LR = fmax(0.25, 55 * ft_Divide_fy) / 100;
			additionData.ρmin_E_M = fmax(0.2, 45 * ft_Divide_fy) / 100;
			additionData.ρmax_E = 0.025;
			additionData.ρmin_sv_E = 0.26 * ft_Divide_fyv;
			additionData.minAs_cRatio_LR = 0.3;
			additionData.minAsRatioContinue = 0;
			break;
		case 4:
			additionData.ξb_E = additionData.ξb;
			additionData.ρmin_E_LR = fmax(0.25, 55 * ft_Divide_fy) / 100;
			additionData.ρmin_E_M = fmax(0.2, 45 * ft_Divide_fy) / 100;
			additionData.ρmax_E = 0.025;
			additionData.ρmin_sv_E = 0.26 * ft_Divide_fyv;
			additionData.minAs_cRatio_LR = 0;
			additionData.minAsRatioContinue = 0;
			break;
		case 5:
			additionData.ξb_E = additionData.ξb;
			additionData.ρmin_E_LR 
				= additionData.ρmin_E_M 
				= additionData.ρmin;
			additionData.ρmax_E = additionData.ρmax;
			additionData.ρmin_sv_E = additionData.ρmin_sv;
			additionData.minAs_cRatio_LR = 0;
			additionData.minAsRatioContinue = 0;
			break;
		default:
			std::cerr << "未知抗震构造等级！" << std::endl;
			break;
		}
	}
}