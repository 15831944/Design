//#include "sqlite3.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>

#include <map>

#include "beam.h"
#include "section.h"
#include "material.h"
#include "xxt.h"

//[portotype]
void test();
///准备数据
void prepareInfo(std::map<double, Concrete*>& concreteMap, std::map<double, Rebar*>& rebarMap);
///获取梁信息
void getInfo(Beam& beam, std::map<double, Concrete*>& concreteMap, std::map<double, Rebar*>& rebarMap);


int main(){
//	test();
	std::map<double, Concrete*> concreteMap;//砼材料表
	std::map<double, Rebar*> rebarMap;//钢筋材料表
	prepareInfo(concreteMap, rebarMap);
	Beam beam;
	while(true){
		getInfo(beam, concreteMap, rebarMap);
		beam.design();
	}
	return 0;
}

void getInfo(Beam& beam, std::map<double, Concrete*>& concreteMap, std::map<double, Rebar*>& rebarMap){
	std::cout << "安全系数、抗震等级、抗震构造等级" << std::endl;
	double γ0;
	int Nfb, Nfb_gz;
	std::cin >> γ0 >> Nfb >> Nfb_gz;
	beam.setCalculateParameter(γ0, Nfb, Nfb_gz);

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
	beam.setMaterial(concretePt, rebarLPt, rebarSPt);

	std::cout << "N V2 V3 T M2 M3" << std::endl;
	double n, v2, v3, t, m2, m3;
	std::cin >> n >> v2 >> v3 >> t >> m2 >> m3;
	beam.setForce(n, v2, v3, t, m2, m3);
/*	stringstream ss;
	ss << "300 700 14.3 360 360";
	cout << "b h fc fy fyv" << endl;
//	ss >> beam.b >> beam.h >> beam.fc >> beam.fy >> beam.fyv;
	cout << "f1 f2 f3 m1 m2 m3" << endl;
	ss.clear();
	ss << "100 600 800 200 600 1200";
//	ss >> beam.f1 >> beam.f2 >> beam.f3 >> beam.m1 >> beam.m2 >> beam.m3;
*/
}

void prepareInfo(std::map<double, Concrete*>& concreteMap, std::map<double, Rebar*>& rebarMap){
	//初始化砼材料
	for(int i = 15; i <= 80; i +=5){
		double curName = i;
		Concrete* curConcrete = new Concrete(curName);
		concreteMap.insert(concreteMap.end(), std::pair<double, Concrete*>(curName, curConcrete));
	}
	//初始化钢筋材料
	double rebar[4] = {300, 335, 400, 500};
	for each(double it in rebar){
		double curName = it;
		Rebar* curRebar = new Rebar(curName);
		rebarMap.insert(rebarMap.end(), std::pair<double, Rebar*>(curName, curRebar));
	}
}
/*
void test()
{
  sqlite3* ptDataBase;//数据库指针
  char* path = ".\\test.db";
  int stage = sqlite3_open(path, &ptDataBase);
  sqlite3_close(ptDataBase);
}*/