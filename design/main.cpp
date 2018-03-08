#include "sqlite3.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>

#include <vector>
#include <map>
#include <set>

#include "design.h"
#include "beam.h"
#include "section.h"
#include "material.h"
#include "force_effect.h"
#include "xxt.h"

//[portotype]
void test();
///准备数据
void prepareInfo
	(std::map<double, Concrete*>& concreteMap
	, std::map<double, Rebar*>& rebarMap
	, std::map<double, Steel*>& steelMap
	, std::vector<std::string>& factorFC
	, std::vector<std::string>& factorNC
	, std::vector<std::string>& factorQPC
	);
///获取梁信息
void getInfo
	(Beam& beam
	, std::map<double, Concrete*>& concreteMap
	, std::map<double, Rebar*>& rebarMap
	, std::map<double, Steel*>& steelMap
	, std::set<Section*>& sectionSet
	, std::vector<std::string>& factorFC
	, std::vector<std::string>& factorNC
	, std::vector<std::string>& factorQPC
	);

int main(){
	//[]stringstream对于字母e也识别为了数字，怎么处理？
	std::stringstream ss;
	ss << "1.2E 1.4e 1.41";
	double a, b, c;
	ss >> a >> b >> c;

//	test();
	std::set<Section*> sectionSet;//截面表//[]以后还是得用map对位编号
	std::map<double, Concrete*> concreteMap;//砼材料表
	std::map<double, Rebar*> rebarMap;//钢筋材料表
	std::map<double, Steel*> steelMap;//钢材表
	std::vector<std::string> factorFC;//基本组合系数表
	std::vector<std::string> factorNC;//标准组合系数表
	std::vector<std::string> factorQPC;//准永久组合系数表

	prepareInfo(concreteMap, rebarMap, steelMap, factorFC, factorNC, factorQPC);
	while(true){
		Beam beam;
		getInfo(beam, concreteMap, rebarMap, steelMap, sectionSet, factorFC, factorNC, factorQPC);
		beam.calcForceData();
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
, std::vector<std::string>& factorFC
, std::vector<std::string>& factorNC
, std::vector<std::string>& factorQPC
){
	//初始化砼材料
	for (int i = 15; i <= 80; i += 5)
	{
		double curName = i;
		Concrete* curConcrete = new Concrete(curName);
		concreteMap.insert(concreteMap.end(), std::pair<double, Concrete*>(curName, curConcrete));
	}
	//初始化钢筋材料
	double rebar[4] = { 300, 335, 400, 500 };
	for each(double it in rebar)
	{
		double curName = it;
		Rebar* curRebar = new Rebar(curName);
		rebarMap.insert(rebarMap.end(), std::pair<double, Rebar*>(curName, curRebar));
	}
	//初始化钢材
	double steel[4] = { 235, 345, 390, 420 };
	for each(double it in steel)
	{
		double curName = it;
		Steel* curSteel = new Steel(curName);
		steelMap.insert(steelMap.end(), std::pair<double, Steel*>(curName, curSteel));
	}
	//初始化基本组合系数表
	//注意保证单工况名称与这里的名称能对上，否则单工况内力取0
	factorFC.insert(factorFC.end(), "1.35D+0.98L");
	factorFC.insert(factorFC.end(), "1.2D+0.6L+1.3E");
	factorFC.insert(factorFC.end(), "1D+1AD");
	//初始化标准组合系数表
	factorNC.insert(factorNC.end(), "1D+1L");
	//初始化准永久组合系数表
	factorQPC.insert(factorQPC.end(), "1D+0.6L");
}

void getInfo
(Beam& beam
, std::map<double, Concrete*>& concreteMap
, std::map<double, Rebar*>& rebarMap
, std::map<double, Steel*>& steelMap
, std::set<Section*>& sectionSet
, std::vector<std::string>& factorFC
, std::vector<std::string>& factorNC
, std::vector<std::string>& factorQPC
){
	beam.setCalculateParameter(1.0, E_NFB::E_NFB_NULL, E_NFB::E_NFB_NULL);
	beam.setBeamType((E_BeamType)0);
	Section* section = new RectSection(300, 700);
	beam.setSection(section, 20);
	Concrete* concretePt = getMapValueClassPt(concreteMap, 30.0);
	Rebar* rebarLPt = getMapValueClassPt(rebarMap, 400.0);
	Rebar* rebarSPt = getMapValueClassPt(rebarMap, 400.0);
	Steel* steelPt = getMapValueClassPt(steelMap, 235.0);
	beam.setMaterial(concretePt, rebarLPt, rebarSPt, steelPt);

	//此阶段输入单工况内力，其中CaseData由 内力、标识符 组成，目前单工况map放在heap中传指针，我没想出更好的办法
	std::map<std::string, CaseData>* caseMap = new std::map<std::string, CaseData>();//单工况内力放在heap中传入指针给Beam，在Beam中析构
	CaseData curCaseData = CaseData(Force(100, 200, 200, 200, 500, 600), E_SingleCaseType::E_SCT_DEAD);;
	caseMap->insert(caseMap->end(), std::pair<std::string, CaseData>("D", curCaseData));
	curCaseData.setData(Force(100, 200, 200, 200, 500, 600), E_SingleCaseType::E_SCT_LIVE);
	caseMap->insert(caseMap->end(), std::pair<std::string, CaseData>("L", curCaseData));
	curCaseData.setData(Force(100, 200, 200, 200, 500, 600), E_SingleCaseType::E_SCT_AD);
	caseMap->insert(caseMap->end(), std::pair<std::string, CaseData>("AD", curCaseData));
	curCaseData.setData(Force(100, 200, 200, 200, 500, 600), E_SingleCaseType::E_SCT_E);
	caseMap->insert(caseMap->end(), std::pair<std::string, CaseData>("E", curCaseData));

	beam.setForceData(caseMap, &factorFC, &factorNC, &factorQPC);
	return;
	/*-----以上为临时测试内容-----*/

	/*
	std::cout << "安全系数、抗震等级、抗震构造等级" << std::endl;
	double γ0;
	E_NFB Nfb, Nfb_gz;
	std::cin >> γ0 >> Nfb >> Nfb_gz;
	beam.setCalculateParameter(γ0, Nfb, Nfb_gz);

	std::cout << "梁类型：0-框架梁；1-非框架梁；2-连梁；3-转换梁" << std::endl;
	int beamType;
	std::cin >> beamType;
	beam.setBeamType((E_BeamType)beamType);//[]这里非得写个强转，否则编不过？

	std::cout << "宽、高、保护层厚度" << std::endl;
	double b, h, c;//宽、高、保护层厚度
	std::cin >> b >> h >> c;
	Section* section = new RectSection(b, h);
	beam.setSection(section, c);
	
	std::cout << "砼、纵筋、箍筋、钢骨材料等级" << std::endl;
	double concreteName, rebarL, rebarS, skeleton;//砼、纵筋、箍筋、钢骨材料
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
  sqlite3* ptDataBase;//数据库指针
  char* path = ".\\test.db";
  int stage = sqlite3_open(path, &ptDataBase);
  sqlite3_close(ptDataBase);
}