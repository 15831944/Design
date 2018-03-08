#include <iostream>
#include <cmath>
#include "design.h"
#include "xxt.h"
#include "concrete_design.h"//砼构件计算公式

const double AS_SINGLE = 22.5;//单筋保护层额外厚度as=c+AS_SINGLE按10箍筋25纵筋考虑
const double AS_DUAL = 47.5;//双筋保护层额外厚度as=c+AS_DUAL按10箍筋25纵筋考虑
const double ρ_LIMIT = 0.01;//控制是否按双排配筋计算的配筋率界限，ρ<ρ_LIMIT单排，ρ≥ρ_LIMIT双排

//[portotype]


Design::Design(){}

Design::~Design(){}

//*------------------------------------*//

DesignBeam::DesignBeam(Beam* beam)
{
	this->beam = beam;
	this->data = &(beam->data);
}

DesignBeam::~DesignBeam(){}

void DesignBeam::setData(void* data)
{
	this->beam = (Beam*)data;
	this->data = &(this->beam->data);
}

void DesignBeam::design()
{
	prepare();
	designULS();//承载能力极限状态验算
	designSLS();//正常使用极限状态验算
}

void DesignBeam::prepare()
{
	setParameter();
	setParameterAD();
	setParameterE();
	beam->m_result.resize(beam->forceData.m_FundamentalCombination.size());
}

void DesignBeam::designULS()
{
	for(int i = 0; i < beam->forceData.m_FundamentalCombination.size(); i++)
	{
		designSection(beam->forceData.m_FundamentalCombination[i].force, beam->m_result[i]);
	}
}

void DesignBeam::designSLS()
{
	//[]待添加
}

void DesignBeam::designSection(const Force& force, Beam::Result& result)
{
	designM(force, result);
	designV(force, result);
}

void DesignBeam::designM(const Force& force, Beam::Result& result)
{
	double γRE = 1;//[]待改，根据组合类型确定
	double ξb = additionData.ξb;//[]待改，根据组合类型确定
	double ρmin = additionData.ρmin;//[]待改，根据组合类型确定
	switch (beam->data.section->getType())
	{
	case E_Section::E_S_RECT_SECTION:
		{//[]非要加{}才不报错吗？初始化操作由“case”标签跳过
		RectSection* section = (RectSection*)data->section;
		designM_Rect(//矩形截面配筋设计
			force.M3,
			section->get_b(), section->get_h(), data->c,
			data->concrete->get_fc(), data->longitudinal->get_fy(), data->longitudinal->get_fy_c(),
			γRE, data->γ0, data->concrete->get_α1(), ξb,
			result.x, result.As, result.As_c, result.ρ, result.ρc);
			break;
		}
	case E_Section::E_S_CIRCLE_SECTION:
		{
		CircleSection* section = (CircleSection*)data->section;
		break;
		}
	default:
		std::cerr << "未知截面！" << std::endl;
		break;
	}
}

void DesignBeam::designV(const Force& force, Beam::Result& result)
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
	if (data->beamType == E_BeamType::E_BT_TRANSFER_BEAM)
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
			break;
		case 1:
			additionData.ξb_E = fmin(0.25, additionData.ξb);
			additionData.ρmin_E_LR
				= additionData.ρmin_E_M
				= 0.005;
			additionData.ρmax_E = 0.025;
			additionData.ρmin_sv_E = 1.2 * ft_Divide_fyv;
			additionData.minAs_cRatio_LR = 0.5;
			additionData.minAsRatioContinue = 0.25;
			break;
		case 2:
			additionData.ξb_E = fmin(0.35, additionData.ξb);
			additionData.ρmin_E_LR
				= additionData.ρmin_E_M
				= 0.004;
			additionData.ρmax_E = 0.025;
			additionData.ρmin_sv_E = 1.1 * ft_Divide_fyv;
			additionData.minAs_cRatio_LR = 0.3;
			additionData.minAsRatioContinue = 0.25;
			break;
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
			break;
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