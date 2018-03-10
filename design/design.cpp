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
}

DesignBeam::~DesignBeam(){}

void DesignBeam::setData(void* data)
{
	this->beam = (Beam*)data;
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
}

void DesignBeam::designULS()
{
	for each(BeamSection curBeamSection in beam->sections)
	{//遍历梁的所有验算截面
		int numFC = curBeamSection.forceData.m_FundamentalCombination.size();//当前截面基本组合数
		curBeamSection.m_resultFC.resize(numFC);
		for (int i = 0; i < numFC; i++)
		{//遍历每个验算截面的所有基本组合
			designSection
				(curBeamSection.section
				, curBeamSection.sectionLocation
				, curBeamSection.forceData.m_FundamentalCombination[i]
				, curBeamSection.m_resultFC[i]
				);
		}
	}
}

void DesignBeam::designSLS()
{
	//[]待添加
}

void DesignBeam::designSection
(Section* section
, E_BeamSectionLocation sectionLocation
, const ForceData& forceData
, BeamSection::ResultFC& resultFC)
{
	switch (forceData.combinationType)
	{//根据不同的基本组合类型设置相应的计算参数，采用统一的函数进行计算
	case E_CombinationType::E_CT_LOAD:
		setDesignParameterN(sectionLocation);
		break;
	case E_CombinationType::E_CT_AD:
		setDesignParameterAD(sectionLocation);
		break;
	case E_CombinationType::E_CT_SEISMIC:
		setDesignParameterE(sectionLocation);
		break;
	default:
		std::cerr << forceData.combinationType << "为不支持的弯矩设计组合类型" << std::endl;
		return;
		break;
	}
	designM(section, forceData, resultFC);
	designV(section, forceData, resultFC);
}

void DesignBeam::setDesignParameterN(E_BeamSectionLocation sectionLocation)
{
	ξb = additionData.ξb;
	γRE = 1.0;
	ρmin = additionData.ρmin;
	ρmax = additionData.ρmax;
	ρmin_sv = additionData.ρmin_sv;
}

void DesignBeam::setDesignParameterAD(E_BeamSectionLocation sectionLocation)
{
	ξb = additionData.ξb_AD;
	γRE = 1.0;
	ρmin = additionData.ρmin_AD;
	ρmax = additionData.ρmax_AD;
	ρmin_sv = additionData.ρmin_sv_AD;
}

void DesignBeam::setDesignParameterE(E_BeamSectionLocation sectionLocation)
{
	ξb = additionData.ξb_E;
	γRE = additionData.γRE_M;
	switch (sectionLocation)
	{
	case E_BSL_LR:
		ρmin = additionData.ρmin_E_LR;
		break;
	case E_BSL_M:
		ρmin = additionData.ρmin_E_M;
		break;
	default:
		std::cerr << sectionLocation << "为错误截面位置类型" << std::endl;
		break;
	}
	ρmax = additionData.ρmax_E;
	ρmin_sv = additionData.ρmin_sv_E;
}

void DesignBeam::designM(Section* section, const ForceData& forceData, BeamSection::ResultFC& resultFC)
{
	switch (section->getType())
	{
	case E_Section::E_S_RECT_SECTION:
	{//[XXT]有时候报错 “初始化操作由“case”标签跳过” 只能通过给case加{}解决
		RectSection* curSection = (RectSection*)section;
		designM_Rect(//矩形截面配筋设计
			forceData.force.M3,
			curSection->get_b(), curSection->get_h(), beam->c,
			beam->concrete->get_fc(), beam->longitudinal->get_fy(), beam->longitudinal->get_fy_c(),
			γRE, beam->γ0, beam->concrete->get_α1(), ξb,
			resultFC.x, resultFC.As, resultFC.As_c, resultFC.ρ, resultFC.ρc);
		break;
	}
	case E_Section::E_S_CIRCLE_SECTION:
	{
		CircleSection* curSection = (CircleSection*)section;
		break;
	}
	default:
		std::cerr << "未知截面！" << std::endl;
		break;
	}
}

void DesignBeam::designV(Section* section, const ForceData& forceData, BeamSection::ResultFC& resultFC)
{
	
}

void DesignBeam::setParameter()
{
	double ft_Divide_fy = beam->concrete->get_ft() / beam->longitudinal->get_fy();
	double ft_Divide_fyv = beam->concrete->get_ft() / beam->stirrup->get_fyv();
	additionData.ξb = beam->concrete->get_β1() / (1 + beam->longitudinal->get_fy() / (beam->longitudinal->get_E() * beam->concrete->get_εcu()));
	additionData.ρmin = fmax(0.2, 45 * ft_Divide_fy) / 100; 
	additionData.ρmax = 0.0275;
	additionData.ρmin_sv = 0.24 * ft_Divide_fyv;
	if (beam->beamType == E_BeamType::E_BT_TRANSFER_BEAM)
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
	if(beam->concrete->get_fcuk() < 35 + EPSILON)
	{
		return 0.0025;
	}
	else if(beam->concrete->get_fcuk() < 55 + EPSILON)
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
	if(beam->concrete->get_fcuk() < 25 + EPSILON)
	{
		return beam->longitudinal->getName() < 335 + EPSILON ? 0.022 : 0.02;
	}
	else
	{
		return beam->longitudinal->getName() < 335 + EPSILON ? 0.025 : 0.024;
	}
}

void DesignBeam::setParameterE()
{
	additionData.γRE_M = 0.75;
	additionData.γRE_V = 0.85;
	double ft_Divide_fy = beam->concrete->get_ft() / beam->longitudinal->get_fy();
	double ft_Divide_fyv = beam->concrete->get_ft() / beam->stirrup->get_fyv();
	switch (beam->beamType)
	{
	//转换梁
	case E_BeamType::E_BT_TRANSFER_BEAM:
		switch (beam->Nfb_gz)
		{
		case E_NFB::E_NFB_LV0:
			additionData.ξb_E = fmin(0.25, additionData.ξb);
			additionData.ρmin_E_LR
				= additionData.ρmin_E_M
				= 0.006;
			additionData.ρmin_sv_E = 1.3 * ft_Divide_fyv;
			additionData.ρmax_E = 0.025;
			additionData.minAs_cRatio_LR = 0.5;
			additionData.minAsRatioContinue = 0.25;
			break;
		case E_NFB::E_NFB_LV1:
			additionData.ξb_E = fmin(0.25, additionData.ξb);
			additionData.ρmin_E_LR
				= additionData.ρmin_E_M
				= 0.005;
			additionData.ρmax_E = 0.025;
			additionData.ρmin_sv_E = 1.2 * ft_Divide_fyv;
			additionData.minAs_cRatio_LR = 0.5;
			additionData.minAsRatioContinue = 0.25;
			break;
		case E_NFB::E_NFB_LV2:
			additionData.ξb_E = fmin(0.35, additionData.ξb);
			additionData.ρmin_E_LR
				= additionData.ρmin_E_M
				= 0.004;
			additionData.ρmax_E = 0.025;
			additionData.ρmin_sv_E = 1.1 * ft_Divide_fyv;
			additionData.minAs_cRatio_LR = 0.3;
			additionData.minAsRatioContinue = 0.25;
			break;
		case E_NFB::E_NFB_LV3:
		case E_NFB::E_NFB_LV4:
			std::cerr << "转换梁抗震构造等级至少为二级！" << std::endl;
			break;
		case E_NFB::E_NFB_NULL:
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
		if (beam->Nfb != E_NFB::E_NFB_NULL || beam->Nfb_gz != E_NFB::E_NFB_NULL) std::cerr << "非框架梁包含抗震等级！" << std::endl;
	default:
		switch (beam->Nfb_gz)
		{
		case E_NFB::E_NFB_LV0:
			additionData.ξb_E = fmin(0.25, additionData.ξb);
			additionData.ρmin_E_LR = fmax(0.4, 80 * ft_Divide_fy) / 100;
			additionData.ρmin_E_M = fmax(0.3, 65 * ft_Divide_fy) / 100;
			additionData.ρmax_E = 0.025;
			additionData.ρmin_sv_E = 0.33 * ft_Divide_fyv;
			additionData.minAs_cRatio_LR = 0.5;
			additionData.minAsRatioContinue = 0.25;
			break;
		case E_NFB::E_NFB_LV1:
			additionData.ξb_E = fmin(0.25, additionData.ξb);
			additionData.ρmin_E_LR = fmax(0.4, 80 * ft_Divide_fy) / 100;
			additionData.ρmin_E_M = fmax(0.3, 65 * ft_Divide_fy) / 100;
			additionData.ρmax_E = 0.025;
			additionData.ρmin_sv_E = 0.3 * ft_Divide_fyv;
			additionData.minAs_cRatio_LR = 0.5;
			additionData.minAsRatioContinue = 0.25;
			break;
		case E_NFB::E_NFB_LV2:
			additionData.ξb_E = fmin(0.35, additionData.ξb);
			additionData.ρmin_E_LR = fmax(0.3, 65 * ft_Divide_fy) / 100;
			additionData.ρmin_E_M = fmax(0.25, 55 * ft_Divide_fy) / 100;
			additionData.ρmax_E = 0.025;
			additionData.ρmin_sv_E = 0.28 * ft_Divide_fyv;
			additionData.minAs_cRatio_LR = 0.3;
			additionData.minAsRatioContinue = 0.25;
			break;
		case E_NFB::E_NFB_LV3:
			additionData.ξb_E = fmin(0.35, additionData.ξb);
			additionData.ρmin_E_LR = fmax(0.25, 55 * ft_Divide_fy) / 100;
			additionData.ρmin_E_M = fmax(0.2, 45 * ft_Divide_fy) / 100;
			additionData.ρmax_E = 0.025;
			additionData.ρmin_sv_E = 0.26 * ft_Divide_fyv;
			additionData.minAs_cRatio_LR = 0.3;
			additionData.minAsRatioContinue = 0;
			break;
		case E_NFB::E_NFB_LV4:
			additionData.ξb_E = additionData.ξb;
			additionData.ρmin_E_LR = fmax(0.25, 55 * ft_Divide_fy) / 100;
			additionData.ρmin_E_M = fmax(0.2, 45 * ft_Divide_fy) / 100;
			additionData.ρmax_E = 0.025;
			additionData.ρmin_sv_E = 0.26 * ft_Divide_fyv;
			additionData.minAs_cRatio_LR = 0;
			additionData.minAsRatioContinue = 0;
			break;
		case E_NFB::E_NFB_NULL:
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