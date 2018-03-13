#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <set>

#include "ConcreteElement.h"
#include "Section.h"
#include "Material.h"
#include "Design.h"
#include "ForceEffect.h"
#include "DataBase.h"

//[portotype]
void test();
///׼������
void prepareTables
	(std::map<double, Concrete*>& concreteMap
	, std::map<double, Rebar*>& rebarMap
	, std::map<double, Steel*>& steelMap
	, std::map<std::string, ForceEffect::E_SingleCaseType>& caseMap
	, std::vector<std::string>& factorFC
	, std::vector<std::string>& factorNC
	, std::vector<std::string>& factorQPC);
///��������ƻ�����Ϣ
void setBasicDesignInfo
	(double ��0
	, std::map<std::string, ForceEffect::E_SingleCaseType> caseMap
	, std::vector<std::string> factorFC
	, std::vector<std::string> factorNC
	, std::vector<std::string> factorQPC);
///��ȡ����Ϣ
void getBeamInfo
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
	test();
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

	prepareTables(concreteMap, rebarMap, steelMap, caseMap, factorFC, factorNC, factorQPC);
	std::cout << "��ȫϵ��=";
	double ��0 = 1;
	std::cin >> ��0;
	setBasicDesignInfo(��0, caseMap, factorFC, factorNC, factorQPC);
	system("pause");

	while(true)
	{
		Beam beam(Beam::E_BeamType::E_BT_FRAME_BEAM, 3);
		getBeamInfo(beam, concreteMap, rebarMap, steelMap, sectionSet);
		beam.calcCombineForceData();
		DesignBeam designBeam;
		designBeam.setData(&beam);
		designBeam.design();

		std::cout << std::string(50, '-') << std::endl;
		beam.showResult();

		system("pause");
		
	}
	return 0;
}

void prepareTables
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
	factorFC.insert(factorFC.end(), "1.35D+0.98L");
	factorFC.insert(factorFC.end(), "1.2D+0.6L+1.3EX");
	factorFC.insert(factorFC.end(), "1D+1AD");
	//��ʼ����׼���ϵ����
	factorNC.insert(factorNC.end(), "1D+1L");
	//��ʼ��׼�������ϵ����
	factorQPC.insert(factorQPC.end(), "1D+0.6L");
}

void setBasicDesignInfo
(double ��0
, std::map<std::string, ForceEffect::E_SingleCaseType> caseMap
, std::vector<std::string> factorFC
, std::vector<std::string> factorNC
, std::vector<std::string> factorQPC)
{
	ConcreteElement::set��0(��0);
	ConcreteElement::setCaspMap(&caseMap);
	ConcreteElement::setFactorFC_Str(&factorFC);
	ConcreteElement::setFactorNC_Str(&factorNC);
	ConcreteElement::setFactorQPC_Str(&factorQPC);
	ConcreteElement::printAllFactorTables();
}

void getBeamInfo
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
	std::vector<BeamSection::E_BeamSectionLocation> sectionLocationTypes;
	for (int i = 0; i < beam.getSectionNumber(); i++)
	{
		sections.insert(sections.end(), section);
		sectionLocationTypes.insert(sectionLocationTypes.end(), BeamSection::E_BeamSectionLocation::E_BSL_LR);
	}
	beam.setSection(sections, 20, 6000, 3000);
	//beam.setSection(sections, sectionLocationTypes, 20, 6000, 3000);//[?]ϵͳ����class BeamSection��ǰ��������

	Concrete* concretePt = getMapValueClassPt(concreteMap, 30.0);
	Rebar* rebarLPt = getMapValueClassPt(rebarMap, 400.0);
	Rebar* rebarSPt = getMapValueClassPt(rebarMap, 400.0);
	Steel* steelPt = getMapValueClassPt(steelMap, 235.0);
	beam.setMaterial(concretePt, rebarLPt, rebarSPt, steelPt);

	//�˽׶������������浥����������Beam->BeamSection->ForceEffect�е�m_caseMap����һ�������Ŀ���
	std::vector<std::map<std::string, Force>> caseMaps(beam.getSectionNumber());

	for (int i = 0; i < beam.getSectionNumber(); i++){
		std::map<std::string, Force> caseMap;
		Force curCaseForce = Force(50, 100, 100, 100, 200, 300);
		caseMap.insert(caseMap.end(), std::pair<std::string, Force>("D", curCaseForce));
		curCaseForce = Force(10, 20, 20, 20, 50, 60);
		caseMap.insert(caseMap.end(), std::pair<std::string, Force>("L", curCaseForce));
		curCaseForce = Force(100, 200, 200, 200, 500, 600);
		caseMap.insert(caseMap.end(), std::pair<std::string, Force>("AD", curCaseForce));
		curCaseForce = Force(100, 200, 200, 200, 500, 600);
		caseMap.insert(caseMap.end(), std::pair<std::string, Force>("EX", curCaseForce));
		caseMaps[i] = caseMap;
	}

	beam.setCaseMap(caseMaps);
	return;
	/*-----����Ϊ��ʱ��������-----*/
}

void test()
{
	DataBase dataBase;
	std::string path = ".\\test.db";
	dataBase.setPath(path);
	dataBase.open();
	std::vector<std::string> columnNameAndTypes = { "Name TEXT" };// , "Age INTEGER", "Address TEXT"

	std::vector<std::string> primaryKey = { "Name" };
	dataBase.createTable("Count", columnNameAndTypes);
	dataBase.addUser("Jerry", "30");
	dataBase.modifyUser("Jerry", "25");
	dataBase.deleteUser("Jerry");
	dataBase.close();
  
  
}