#include "ConcreteElement.h"

#include <iostream>
#include <string>

ConcreteElement::ConcreteElement(){}

ConcreteElement::~ConcreteElement(){}

void ConcreteElement::set��0(double ��0)//[XXT]��̬��Ա����дʵ�ֵ�ʱ�򲻼�static
{
	ConcreteElement::��0 = ��0;//[XXT]��̬��Ա����������this
}

void ConcreteElement::setCaspMap(std::map<std::string, ForceEffect::E_SingleCaseType>* caseMap)
{
	ConcreteElement::m_caseMap = caseMap;
}

void ConcreteElement::setFactorFC_Str(std::vector<std::string>* factorFC)
{
	ConcreteElement::m_FactorFC_Str = factorFC;
	parserFactorFC();
}

void ConcreteElement::setFactorNC_Str(std::vector<std::string>* factorNC)
{
	ConcreteElement::m_FactorNC_Str = factorNC;
	parserFactorNC();
}

void ConcreteElement::setFactorQPC_Str(std::vector<std::string>* factorQPC)
{
	ConcreteElement::m_FactorQPC_Str = factorQPC;
	parserFactorQPC();
}

void ConcreteElement::parserFactorFC()
{
	parserCombination(m_FactorFC_Str, m_caseMap, m_FactorFC, ForceEffect::E_CombinationType::E_CT_FC);
}

void ConcreteElement::parserFactorNC()
{
	parserCombination(m_FactorNC_Str, m_caseMap, m_FactorNC, ForceEffect::E_CombinationType::E_CT_NC);
}

void ConcreteElement::parserFactorQPC()
{
	parserCombination(m_FactorQPC_Str, m_caseMap, m_FactorQPC, ForceEffect::E_CombinationType::E_CT_QPC);
}

void ConcreteElement::printAllFactorTables()
{
	std::cout << "�������" << std::endl;
	printFactorTable(m_FactorFC);
	std::cout << "��׼���" << std::endl;
	printFactorTable(m_FactorNC);
	std::cout << "׼�������" << std::endl;
	printFactorTable(m_FactorQPC);
}

void ConcreteElement::printFactorTable(const std::vector<ForceEffect::CombineExp>& factorTable)
{
	for each(ForceEffect::CombineExp curCombineExp in factorTable)
	{
		for each(std::pair<double, std::string> curCombinePair in curCombineExp.casePairData)
		{
			std::cout << curCombinePair.first << "(" << curCombinePair.second << ")" << " + ";
		}
		switch (curCombineExp.combineType)
		{
		case ForceEffect::E_CombinationType::E_CT_FC:
			std::cout << "�������";
			break;
		case ForceEffect::E_CombinationType::E_CT_LOAD:
			std::cout << "�ǵ������";
			break;
		case ForceEffect::E_CombinationType::E_CT_SEISMIC:
			std::cout << "�������";
			break;
		case ForceEffect::E_CombinationType::E_CT_AD:
			std::cout << "�˷����";
			break;
		case ForceEffect::E_CombinationType::E_CT_NC:
			std::cout << "��׼���";
			break;
		case ForceEffect::E_CombinationType::E_CT_QPC:
			std::cout << "׼�������";
			break;
		default:
			std::cout << "δʶ�����ϣ�";
			break;
		}
		std::cout << std::endl;
	}
}

void ConcreteElement::parserCombination//���� �������ϵ����(string��ʽ)�����������ͱ� ���ɶ�Ӧ�ĺ������ϵ����(������)
(std::vector<std::string>* factorTable_Str
, std::map<std::string, ForceEffect::E_SingleCaseType>*& caseMap
, std::vector<ForceEffect::CombineExp>& factorTable
, ForceEffect::E_CombinationType combineType)
{
	for each(std::string line in *factorTable_Str)
	{
		parserCombinationExp(line, factorTable, combineType);
	}
}

void ConcreteElement::parserCombinationExp//����������ϱ��ʽ
(const std::string& line
, std::vector<ForceEffect::CombineExp>& factorTable
, ForceEffect::E_CombinationType combineType)
{
	ForceEffect::CombineExp::TD_CombinePair combineExp;//vector(������ϵ��������������)
	ForceEffect::E_SingleCaseType maxSingleCaseType = ForceEffect::E_SingleCaseType::E_SCT_DEAD;//������󵥹��������жϺ����������

	std::string caseName;//����������
	double factor = 0;//���������ϵ��
	int indexStart = line.size() - 1;//��ȡ�ַ�����ʼλ��
	int indexEnd = line.size() - 1;//��ȡ�ַ�������λ��
	bool gettingCaseName = true;//��ʾ�Ƿ�������ȡcaseName
	bool gettingFactor = false;//��ʾ�Ƿ�������ȡfactor
	//��line���Ž��н���
	for (int i = line.size() - 1; i >= 0; i--)
	{
		if (gettingCaseName && isdigit(line[i]))
		{//�ҵ�caseName��ʼλ��
			gettingCaseName = false;
			gettingFactor = true;
			indexStart = i + 1;//����caseName��startIndex
			caseName = line.substr(indexStart, indexEnd - indexStart + 1);
			caseName.erase(0, caseName.find_first_not_of(" "));//ɾ��ͷ���ո�
			caseName.erase(caseName.find_last_not_of(" ") + 1);//ɾ��β���ո�
			indexEnd = i;//׼����ȡfactor
		}
		
		if (gettingFactor && (line[i] == '+' || line[i] == '-' || i == 0))
		{//�ҵ�factor��ʼλ��			
			gettingCaseName = true;
			gettingFactor = false;
			indexStart = (i == 0) ? i : i + 1;//����factor��startIndex
			std::string factorStr = line.substr(indexStart, indexEnd - indexStart + 1);
			factorStr.erase(0, factorStr.find_first_not_of(" "));//ɾ��ͷ���ո�
			factorStr.erase(factorStr.find_last_not_of(" ") + 1);//ɾ��β���ո�
			factor = std::stod(factorStr);
			if (line[i] == '-') factor = -factor;//�ж��Ƿ�ϵ��Ϊ����
			indexEnd = i -1;
			//��casePairд��combineExp��
			if (m_caseMap->count(caseName) == 1)
			{
				combineExp.insert(combineExp.begin(), std::pair<double, std::string>(factor, caseName));
				if (maxSingleCaseType < m_caseMap->at(caseName)) maxSingleCaseType = m_caseMap->at(caseName);
			}
			factor = 0;
		}
	}

	ForceEffect::E_CombinationType curCombinationType = getCombinationType(combineType, maxSingleCaseType);
	factorTable.insert(factorTable.end(), ForceEffect::CombineExp(curCombinationType, combineExp));
}

ForceEffect::E_CombinationType ConcreteElement::getCombinationType
(ForceEffect::E_CombinationType combineType
, ForceEffect::E_SingleCaseType maxSingleCaseType)
{
	switch (combineType)
	{
	case ForceEffect::E_CombinationType::E_CT_FC:
		switch (maxSingleCaseType)
		{
		case ForceEffect::E_SingleCaseType::E_SCT_AD:
			return ForceEffect::E_CombinationType::E_CT_AD;
			break;
		case ForceEffect::E_SingleCaseType::E_SCT_E:
			return ForceEffect::E_CombinationType::E_CT_SEISMIC;
			break;
		default:
			return ForceEffect::E_CombinationType::E_CT_LOAD;
			break;
		}
		break;
	case ForceEffect::E_CombinationType::E_CT_NC:
		return ForceEffect::E_CombinationType::E_CT_NC;
		break;
	case ForceEffect::E_CombinationType::E_CT_QPC:
		return ForceEffect::E_CombinationType::E_CT_QPC;
		break;
	default:
		std::cerr << "δ֪������ͣ�" << std::endl;
		break;
	}
}

//*------------------------------------*//

Beam::Beam()
{
	this->c = 999;
	this->Lc2 = 0;
	this->Lc3 = 0;
	setMaterial(NULL, NULL, NULL, NULL);
	setCalculateParameter(E_NFB::E_NFB_NULL, E_NFB::E_NFB_NULL);
	setBeamType(E_BeamType::E_BT_BEAM);
	this->sections.resize(SECTION_NUMBER);
}

Beam::~Beam(){}

void Beam::setBeamType(E_BeamType beamType)
{
	this->beamType = beamType;
}

void Beam::setSection(const std::vector<Section*>& sections, double c, double Lc2, double Lc3)
{
	this->c = c;
	this->Lc2 = Lc2;
	this->Lc3 = Lc3;
	for(int i = 0; i < sections.size(); i++)
	{
		this->sections[i].setSection(sections[i]);
	}
}

void Beam::setMaterial(Concrete* concrete, Rebar* rebarL, Rebar* rebarS, Steel* steel)
{
	this->concrete = concrete;
	this->longitudinal = rebarL;
	this->stirrup = rebarS;
	this->skeleton = steel;
}

void Beam::setCalculateParameter(E_NFB Nfb, E_NFB Nfb_gz)
{
	this->Nfb = Nfb;
	this->Nfb_gz = Nfb_gz;
}

void Beam::setCaseMap(const std::vector<std::map<std::string, ForceEffect::CaseForceData>>& caseMaps)
{
	for (int i = 0; i < sections.size(); i++)
	{
		this->sections[i].setCombineForceData(caseMaps[i], &this->m_FactorFC, &this->m_FactorNC, &this->m_FactorQPC);
	}
}

void Beam::calcCombineForceData()
{
	for (int i = 0; i < sections.size(); i++)
	{
		sections[i].calcCombineForceData();
	}
}

void Beam::showResult()
{
	for (int i = 0; i < sections.size(); i++)
	{
		std::cout << "��" << i << "�Ž�����ƽ����" << std::endl;
		sections[i].showResult();
	}
}

//*------------------------------------*//

BeamSection::BeamSection()
{
	//setSection(NULL);
	//setSectionLocationType(E_BeamSectionLocation::E_BSL_NULL);//[]�ܽ�ö�ٳ�ʼ����NULL��
}

BeamSection::~BeamSection(){}

void BeamSection::setSection(Section* section)
{
	this->section = section;
}

void BeamSection::setSectionLocationType(E_BeamSectionLocation sectionLocation)
{
	this->sectionLocation = sectionLocation;
}

void BeamSection::setCombineForceData
(const std::map<std::string, ForceEffect::CaseForceData>& caseMap
, std::vector<ForceEffect::CombineExp>* factorFC
, std::vector<ForceEffect::CombineExp>* factorNC
, std::vector<ForceEffect::CombineExp>* factorQPC
){
	forceData.setCaseMap(caseMap);
	forceData.setFC(factorFC);
	forceData.setNC(factorNC);
	forceData.setQPC(factorQPC);
}

void BeamSection::calcCombineForceData()
{
	if (forceData.hasFC()) forceData.calcFC();
	if (forceData.hasNC()) forceData.calcNC();
	if (forceData.hasQPC()) forceData.calcQPC();
}

void BeamSection::showResult()
{
	for (int i = 0; i < m_resultFC.size(); i++)
	{
		std::cout << "��" << i << "����Ͻ��:" << std::endl;
		std::cout << "������" << std::endl;
		std::cout << "x=" << m_resultFC[i].x << std::endl;
		std::cout << "As=" << m_resultFC[i].As << "  ��=" << m_resultFC[i].�� << std::endl;
		std::cout << "As'=" << m_resultFC[i].As_c << "  ��'=" << m_resultFC[i].��c << std::endl;
		std::cout << "б����" << std::endl;
		std::cout << "Asv=" << m_resultFC[i].Asv << "  ��sv=" << m_resultFC[i].��sv << std::endl;
		std::cout << std::endl;
	}
}