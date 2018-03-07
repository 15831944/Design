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
	);

int main(){
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
		getInfo(beam, concreteMap, rebarMap, steelMap, sectionSet);
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
	factorFC.insert(factorFC.end(), "1.35D+0.98L");
	factorFC.insert(factorFC.end(), "1.2D+1.4L");
	factorFC.insert(factorFC.end(), "1D+1.2L");
	//初始化标准组合系数表
	factorNC.insert(factorNC.end(), "1D+1L");
	//初始化准永久组合系数表
	factorQPC.insert(factorQPC.end(), "1D+1L");
}

void getInfo
(Beam& beam
, std::map<double, Concrete*>& concreteMap
, std::map<double, Rebar*>& rebarMap
, std::map<double, Steel*>& steelMap
, std::set<Section*>& sectionSet
){
	beam.setCalculateParameter(1.0, 5, 5);
	beam.setBeamType((E_BeamType)0);
	Section* section = new RectSection(300, 700);
	beam.setSection(section, 20);
	Concrete* concretePt = getMapValueClassPt(concreteMap, 30.0);
	Rebar* rebarLPt = getMapValueClassPt(rebarMap, 400.0);
	Rebar* rebarSPt = getMapValueClassPt(rebarMap, 400.0);
	Steel* steelPt = getMapValueClassPt(steelMap, 235.0);
	beam.setMaterial(concretePt, rebarLPt, rebarSPt, steelPt);
	beam.setForce(200, 300, 400, 100, 1200, 368);
	beam.setForce(200, 300, 400, 100, 1200, 314);
	return;
	/*-----以上为临时测试内容-----*/

	/*
	std::cout << "安全系数、抗震等级、抗震构造等级" << std::endl;
	double γ0;
	int Nfb, Nfb_gz;
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