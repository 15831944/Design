#include <iostream>
#include <cmath>
#include "design.h"
#include "xxt.h"
#include "concrete_design.h"//�Ź������㹫ʽ

const double AS_SINGLE = 22.5;//������������as=c+AS_SINGLE��10����25�ݽ��
const double AS_DUAL = 47.5;//˫����������as=c+AS_DUAL��10����25�ݽ��
const double ��_LIMIT = 0.01;//�����Ƿ�˫�������������ʽ��ޣ���<��_LIMIT���ţ��ѡݦ�_LIMIT˫��

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
	designULS();//������������״̬����
	designSLS();//����ʹ�ü���״̬����
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
		designSection(beam->forceData.m_FundamentalCombination[i], beam->m_result[i]);
	}
}

void DesignBeam::designSLS()
{
	//[]�����
}

void DesignBeam::designSection(const ForceData& forceData, BeamSection::Result& result)
{
	switch (forceData.combinationType)
	{
	case E_CombinationType::E_CT_LOAD:
		setDesignTypeN();
		break;
	case E_CombinationType::E_CT_AD:
		setDesignTypeAD();
		break;
	case E_CombinationType::E_CT_SEISMIC:
		setDesignTypeE();
		break;
	default:
		std::cerr << forceData.combinationType << "Ϊ��֧�ֵ��������������" << std::endl;
		return;
		break;
	}
	designM(forceData, result);
	designV(forceData, result);
}

void DesignBeam::setDesignTypeN()
{
	��b = additionData.��b;
	��RE = 1.0;
	��min = additionData.��min;
	��max = additionData.��max;
	��min_sv = additionData.��min_sv;
}

void DesignBeam::setDesignTypeAD()
{
	��b = additionData.��b_AD;
	��RE = 1.0;
	��min = additionData.��min_AD;
	��max = additionData.��max_AD;
	��min_sv = additionData.��min_sv_AD;
}

void DesignBeam::setDesignTypeE()
{
	��b = additionData.��b_E;
	��RE = additionData.��RE_M;
	��min = additionData.��min_E_LR;//[]���ݽ���λ�����벻ͬ������//[]����
	��max = additionData.��max_E;
	��min_sv = additionData.��min_sv_E;
}

void DesignBeam::designM(const ForceData& forceData, BeamSection::Result& result)
{
	switch (beam->section->getType())
	{
	case E_Section::E_S_RECT_SECTION:
	{//[XXT]��ʱ�򱨴� ����ʼ�������ɡ�case����ǩ������ ֻ��ͨ����case��{}���
		RectSection* section = (RectSection*)beam->section;
		designM_Rect(//���ν���������
			forceData.force.M3,
			section->get_b(), section->get_h(), beam->c,
			beam->concrete->get_fc(), beam->longitudinal->get_fy(), beam->longitudinal->get_fy_c(),
			��RE, beam->��0, beam->concrete->get_��1(), ��b,
			result.x, result.As, result.As_c, result.��, result.��c);
		break;
	}
	case E_Section::E_S_CIRCLE_SECTION:
	{
		CircleSection* section = (CircleSection*)beam->section;
		break;
	}
	default:
		std::cerr << "δ֪���棡" << std::endl;
		break;
	}
}

void DesignBeam::designV(const ForceData& forceData, BeamSection::Result& result)
{
	
}

void DesignBeam::setParameter()
{
	double ft_Divide_fy = beam->concrete->get_ft() / beam->longitudinal->get_fy();
	double ft_Divide_fyv = beam->concrete->get_ft() / beam->stirrup->get_fyv();
	additionData.��b = beam->concrete->get_��1() / (1 + beam->longitudinal->get_fy() / (beam->longitudinal->get_E() * beam->concrete->get_��cu()));
	additionData.��min = fmax(0.2, 45 * ft_Divide_fy) / 100; 
	additionData.��max = 0.0275;
	additionData.��min_sv = 0.24 * ft_Divide_fyv;
	if (beam->beamType == E_BeamType::E_BT_TRANSFER_BEAM)
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

double DesignBeam::calc_��max_AD()
{//���˷��淶��4.11.8
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
	additionData.��RE_M = 0.75;
	additionData.��RE_V = 0.85;
	double ft_Divide_fy = beam->concrete->get_ft() / beam->longitudinal->get_fy();
	double ft_Divide_fyv = beam->concrete->get_ft() / beam->stirrup->get_fyv();
	switch (beam->beamType)
	{
	//ת����
	case E_BeamType::E_BT_TRANSFER_BEAM:
		switch (beam->Nfb_gz)
		{
		case E_NFB::E_NFB_LV0:
			additionData.��b_E = fmin(0.25, additionData.��b);
			additionData.��min_E_LR
				= additionData.��min_E_M
				= 0.006;
			additionData.��min_sv_E = 1.3 * ft_Divide_fyv;
			additionData.��max_E = 0.025;
			additionData.minAs_cRatio_LR = 0.5;
			additionData.minAsRatioContinue = 0.25;
			break;
		case E_NFB::E_NFB_LV1:
			additionData.��b_E = fmin(0.25, additionData.��b);
			additionData.��min_E_LR
				= additionData.��min_E_M
				= 0.005;
			additionData.��max_E = 0.025;
			additionData.��min_sv_E = 1.2 * ft_Divide_fyv;
			additionData.minAs_cRatio_LR = 0.5;
			additionData.minAsRatioContinue = 0.25;
			break;
		case E_NFB::E_NFB_LV2:
			additionData.��b_E = fmin(0.35, additionData.��b);
			additionData.��min_E_LR
				= additionData.��min_E_M
				= 0.004;
			additionData.��max_E = 0.025;
			additionData.��min_sv_E = 1.1 * ft_Divide_fyv;
			additionData.minAs_cRatio_LR = 0.3;
			additionData.minAsRatioContinue = 0.25;
			break;
		case E_NFB::E_NFB_LV3:
		case E_NFB::E_NFB_LV4:
			std::cerr << "ת����������ȼ�����Ϊ������" << std::endl;
			break;
		case E_NFB::E_NFB_NULL:
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
	//������
	case E_BeamType::E_BT_BEAM:
		if (beam->Nfb != E_NFB::E_NFB_NULL || beam->Nfb_gz != E_NFB::E_NFB_NULL) std::cerr << "�ǿ������������ȼ���" << std::endl;
	default:
		switch (beam->Nfb_gz)
		{
		case E_NFB::E_NFB_LV0:
			additionData.��b_E = fmin(0.25, additionData.��b);
			additionData.��min_E_LR = fmax(0.4, 80 * ft_Divide_fy) / 100;
			additionData.��min_E_M = fmax(0.3, 65 * ft_Divide_fy) / 100;
			additionData.��max_E = 0.025;
			additionData.��min_sv_E = 0.33 * ft_Divide_fyv;
			additionData.minAs_cRatio_LR = 0.5;
			additionData.minAsRatioContinue = 0.25;
			break;
		case E_NFB::E_NFB_LV1:
			additionData.��b_E = fmin(0.25, additionData.��b);
			additionData.��min_E_LR = fmax(0.4, 80 * ft_Divide_fy) / 100;
			additionData.��min_E_M = fmax(0.3, 65 * ft_Divide_fy) / 100;
			additionData.��max_E = 0.025;
			additionData.��min_sv_E = 0.3 * ft_Divide_fyv;
			additionData.minAs_cRatio_LR = 0.5;
			additionData.minAsRatioContinue = 0.25;
			break;
		case E_NFB::E_NFB_LV2:
			additionData.��b_E = fmin(0.35, additionData.��b);
			additionData.��min_E_LR = fmax(0.3, 65 * ft_Divide_fy) / 100;
			additionData.��min_E_M = fmax(0.25, 55 * ft_Divide_fy) / 100;
			additionData.��max_E = 0.025;
			additionData.��min_sv_E = 0.28 * ft_Divide_fyv;
			additionData.minAs_cRatio_LR = 0.3;
			additionData.minAsRatioContinue = 0.25;
			break;
		case E_NFB::E_NFB_LV3:
			additionData.��b_E = fmin(0.35, additionData.��b);
			additionData.��min_E_LR = fmax(0.25, 55 * ft_Divide_fy) / 100;
			additionData.��min_E_M = fmax(0.2, 45 * ft_Divide_fy) / 100;
			additionData.��max_E = 0.025;
			additionData.��min_sv_E = 0.26 * ft_Divide_fyv;
			additionData.minAs_cRatio_LR = 0.3;
			additionData.minAsRatioContinue = 0;
			break;
		case E_NFB::E_NFB_LV4:
			additionData.��b_E = additionData.��b;
			additionData.��min_E_LR = fmax(0.25, 55 * ft_Divide_fy) / 100;
			additionData.��min_E_M = fmax(0.2, 45 * ft_Divide_fy) / 100;
			additionData.��max_E = 0.025;
			additionData.��min_sv_E = 0.26 * ft_Divide_fyv;
			additionData.minAs_cRatio_LR = 0;
			additionData.minAsRatioContinue = 0;
			break;
		case E_NFB::E_NFB_NULL:
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