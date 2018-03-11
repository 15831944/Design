#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <set>

#include "sqlite3.h"

#include "ConcreteElement.h"
#include "Section.h"
#include "Material.h"
#include "Design.h"
#include "ForceEffect.h"

//[portotype]
void test();
///׼������
void prepareInfo
	(std::map<double, Concrete*>& concreteMap
	, std::map<double, Rebar*>& rebarMap
	, std::map<double, Steel*>& steelMap
	, std::map<std::string, ForceEffect::E_SingleCaseType>& caseMap
	, std::vector<std::string>& factorFC
	, std::vector<std::string>& factorNC
	, std::vector<std::string>& factorQPC);
///��ȡ����Ϣ
void getInfo
	(Beam& beam
	, std::map<double, Concrete*>& concreteMap
	, std::map<double, Rebar*>& rebarMap
	, std::map<double, Steel*>& steelMap
	, std::set<Section*>& sectionSet);

//[��̬��Ա��������]
double ConcreteElement::��0;
std::map<std::string, ForceEffect::E_SingleCaseType>* ConcreteElement::m_caseMap;
std::vector<std::string>* ConcreteElement::m_FactorFC_Str;
std::vector<std::string>* ConcreteElement::m_FactorNC_Str;
std::vector<std::string>* ConcreteElement::m_FactorQPC_Str;
std::vector<ForceEffect::CombineExp> ConcreteElement::m_FactorFC;
std::vector<ForceEffect::CombineExp> ConcreteElement::m_FactorNC;
std::vector<ForceEffect::CombineExp> ConcreteElement::m_FactorQPC;

int main(){
//	test();
	//����
	std::set<Section*> sectionSet;//�����//[]�Ժ��ǵ���map��λ���
	//����
	std::map<double, Concrete*> concreteMap;//�Ų��ϱ�
	std::map<double, Rebar*> rebarMap;//�ֽ���ϱ�
	std::map<double, Steel*> steelMap;//�ֲı�
	//����
	std::map<std::string, ForceEffect::E_SingleCaseType> caseMap;//���������ͱ�
	std::vector<std::string> factorFC;//�������ϵ����
	std::vector<std::string> factorNC;//��׼���ϵ����
	std::vector<std::string> factorQPC;//׼�������ϵ����

	prepareInfo(concreteMap, rebarMap, steelMap, caseMap, factorFC, factorNC, factorQPC);
	ConcreteElement::set��0(1.0);//[]�������û��޸�
	ConcreteElement::setCaspMap(&caseMap);
	ConcreteElement::setFactorFC_Str(&factorFC);
	ConcreteElement::setFactorNC_Str(&factorNC);
	ConcreteElement::setFactorQPC_Str(&factorQPC);
	ConcreteElement::printAllFactorTables();
	system("pause");

	while(true){
		Beam beam;
		getInfo(beam, concreteMap, rebarMap, steelMap, sectionSet);
		beam.calcCombineForceData();
		DesignBeam designBeam;
		designBeam.setData(&beam);
		designBeam.design();
		beam.showResult();

		system("pause");
	}
	return 0;
}

void prepareInfo
(std::map<double, Concrete*>& concreteMap
, std::map<double, Rebar*>& rebarMap
, std::map<double, Steel*>& steelMap
, std::map<std::string, ForceEffect::E_SingleCaseType>& caseMap
, std::vector<std::string>& factorFC
, std::vector<std::string>& factorNC
, std::vector<std::string>& factorQPC)
{
	//��ʼ���Ų���
	for (int i = 15; i <= 80; i += 5)
	{
		double curName = i;
		Concrete* curConcrete = new Concrete(curName);
		concreteMap.insert(concreteMap.end(), std::pair<double, Concrete*>(curName, curConcrete));
	}
	//��ʼ���ֽ����
	double rebar[4] = { 300, 335, 400, 500 };
	for each(double it in rebar)
	{
		double curName = it;
		Rebar* curRebar = new Rebar(curName);
		rebarMap.insert(rebarMap.end(), std::pair<double, Rebar*>(curName, curRebar));
	}
	//��ʼ���ֲ�
	double steel[4] = { 235, 345, 390, 420 };
	for each(double it in steel)
	{
		double curName = it;
		Steel* curSteel = new Steel(curName);
		steelMap.insert(steelMap.end(), std::pair<double, Steel*>(curName, curSteel));
	}
	//��ʼ�����������ͱ�//[]�˴�����YJKͳһ
	//[]������ֱ��ʹ��ConcreteElement::caseMap.insert(...)��������
	caseMap.insert(caseMap.end(), std::pair<std::string, ForceEffect::E_SingleCaseType>("D", ForceEffect::E_SingleCaseType::E_SCT_DEAD));
	caseMap.insert(caseMap.end(), std::pair<std::string, ForceEffect::E_SingleCaseType>("L", ForceEffect::E_SingleCaseType::E_SCT_LIVE));
	caseMap.insert(caseMap.end(), std::pair<std::string, ForceEffect::E_SingleCaseType>("WX+", ForceEffect::E_SingleCaseType::E_SCT_WIND));
	caseMap.insert(caseMap.end(), std::pair<std::string, ForceEffect::E_SingleCaseType>("WX-", ForceEffect::E_SingleCaseType::E_SCT_WIND));
	caseMap.insert(caseMap.end(), std::pair<std::string, ForceEffect::E_SingleCaseType>("WY+", ForceEffect::E_SingleCaseType::E_SCT_WIND));
	caseMap.insert(caseMap.end(), std::pair<std::string, ForceEffect::E_SingleCaseType>("WY-", ForceEffect::E_SingleCaseType::E_SCT_WIND));
	caseMap.insert(caseMap.end(), std::pair<std::string, ForceEffect::E_SingleCaseType>("T", ForceEffect::E_SingleCaseType::E_SCT_TEMPERATURE));
	caseMap.insert(caseMap.end(), std::pair<std::string, ForceEffect::E_SingleCaseType>("PS", ForceEffect::E_SingleCaseType::E_SCT_PRESTRESS));
	caseMap.insert(caseMap.end(), std::pair<std::string, ForceEffect::E_SingleCaseType>("AD", ForceEffect::E_SingleCaseType::E_SCT_AD));
	caseMap.insert(caseMap.end(), std::pair<std::string, ForceEffect::E_SingleCaseType>("EX", ForceEffect::E_SingleCaseType::E_SCT_E));
	caseMap.insert(caseMap.end(), std::pair<std::string, ForceEffect::E_SingleCaseType>("EY", ForceEffect::E_SingleCaseType::E_SCT_E));
	caseMap.insert(caseMap.end(), std::pair<std::string, ForceEffect::E_SingleCaseType>("EXY", ForceEffect::E_SingleCaseType::E_SCT_E));//XΪ����˫���������
	caseMap.insert(caseMap.end(), std::pair<std::string, ForceEffect::E_SingleCaseType>("EYX", ForceEffect::E_SingleCaseType::E_SCT_E));//YΪ����˫���������
	//��ʼ���������ϵ����
	//ע�Ᵽ֤����������������������ܶ��ϣ����򵥹�������ȡ0
	factorFC.insert(factorFC.end(), " 1.35 D  +  0.64   WX+    +   0.98   L  ");
	factorFC.insert(factorFC.end(), "1.35D-0.64WX-+0.98L");
	factorFC.insert(factorFC.end(), "1.2D+0.6L+1.3EX");
	factorFC.insert(factorFC.end(), "1.2D+0.6L-1.3EYX");
	factorFC.insert(factorFC.end(), "1D+1AD");
	//��ʼ����׼���ϵ����
	factorNC.insert(factorNC.end(), "1D+1L");
	//��ʼ��׼�������ϵ����
	factorQPC.insert(factorQPC.end(), "1D+0.6L");
}

void getInfo
(Beam& beam
, std::map<double, Concrete*>& concreteMap
, std::map<double, Rebar*>& rebarMap
, std::map<double, Steel*>& steelMap
, std::set<Section*>& sectionSet
){
	beam.setCalculateParameter(E_NFB::E_NFB_NULL, E_NFB::E_NFB_NULL);
	beam.setBeamType((Beam::E_BeamType)0);
	Section* section = new RectSection(300, 700);
	std::vector<Section*> sections;
	for (int i = 0; i < 9; i++)
	{
		sections.insert(sections.end(), section);
	}
	beam.setSection(sections, 20, 6000, 3000);
	Concrete* concretePt = getMapValueClassPt(concreteMap, 30.0);
	Rebar* rebarLPt = getMapValueClassPt(rebarMap, 400.0);
	Rebar* rebarSPt = getMapValueClassPt(rebarMap, 400.0);
	Steel* steelPt = getMapValueClassPt(steelMap, 235.0);
	beam.setMaterial(concretePt, rebarLPt, rebarSPt, steelPt);

	//�˽׶������������浥����������Beam->BeamSection->ForceEffect�е�m_caseMap����һ�������Ŀ���
	std::vector<std::map<std::string, ForceEffect::CaseForceData>> caseMaps(9);

	for (int i = 0; i < 9; i++){
		std::map<std::string, ForceEffect::CaseForceData> caseMap;
		ForceEffect::CaseForceData curCaseForceData = ForceEffect::CaseForceData(Force(100, 200, 200, 200, 500, 600), ForceEffect::E_SingleCaseType::E_SCT_DEAD);;
		caseMap.insert(caseMap.end(), std::pair<std::string, ForceEffect::CaseForceData>("D", curCaseForceData));
		curCaseForceData.setData(Force(100, 200, 200, 200, 500, 600), ForceEffect::E_SingleCaseType::E_SCT_LIVE);
		caseMap.insert(caseMap.end(), std::pair<std::string, ForceEffect::CaseForceData>("L", curCaseForceData));
		curCaseForceData.setData(Force(100, 200, 200, 200, 500, 600), ForceEffect::E_SingleCaseType::E_SCT_AD);
		caseMap.insert(caseMap.end(), std::pair<std::string, ForceEffect::CaseForceData>("AD", curCaseForceData));
		curCaseForceData.setData(Force(100, 200, 200, 200, 500, 600), ForceEffect::E_SingleCaseType::E_SCT_E);
		caseMap.insert(caseMap.end(), std::pair<std::string, ForceEffect::CaseForceData>("E", curCaseForceData));
		caseMaps[i] = caseMap;
	}

	beam.setCaseMap(caseMaps);
	return;
	/*-----����Ϊ��ʱ��������-----*/

	/*
	std::cout << "��ȫϵ��������ȼ���������ȼ�" << std::endl;
	double ��0;
	E_NFB Nfb, Nfb_gz;
	std::cin >> ��0 >> Nfb >> Nfb_gz;
	beam.setCalculateParameter(��0, Nfb, Nfb_gz);

	std::cout << "�����ͣ�0-�������1-�ǿ������2-������3-ת����" << std::endl;
	int beamType;
	std::cin >> beamType;
	beam.setBeamType((E_BeamType)beamType);//[]����ǵ�д��ǿת������಻����

	std::cout << "���ߡ���������" << std::endl;
	double b, h, c, L2, L3;//���ߡ��������ȡ����ֲ�������2��3����ļ��㳤��
	std::cin >> b >> h >> c >> L2 >> L3;
	Section* section = new RectSection(b, h);
	beam.setSection(section, c, L2, L3);
	
	std::cout << "�š��ݽ����ֹǲ��ϵȼ�" << std::endl;
	double concreteName, rebarL, rebarS, skeleton;//�š��ݽ����ֹǲ���
	std::cin >> concreteName >> rebarL >> rebarS >> skeleton;
	Concrete* concretePt = getMapValueClassPt(concreteMap, concreteName);
	Rebar* rebarLPt = getMapValueClassPt(rebarMap, rebarL);
	Rebar* rebarSPt = getMapValueClassPt(rebarMap, rebarS);
	Steel* steelPt = getMapValueClassPt(steelMap, skeleton);
	beam.setMaterial(concretePt, rebarLPt, rebarSPt, steelPt);

	std::cout << "N V2 V3 T M2 M3" << std::endl;
	double n, v2, v3, t, m2, m3;
	std::cin >> n >> v2 >> v3 >> t >> m2 >> m3;
	beam.setForce(n, v2, v3, t, m2, m3);
	*/
}

void test()
{
  sqlite3* ptDataBase;//���ݿ�ָ��
  char* path = ".\\test.db";
  int stage = sqlite3_open(path, &ptDataBase);
  sqlite3_close(ptDataBase);
}