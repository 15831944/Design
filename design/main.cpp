#include <iostream>
#include <sstream>
#include <fstream>
#include <string>

#include <map>

#include "beam.h"
#include "Section.h"
#include "material.h"

struct Beam;

using namespace std;

//[portotype]
///准备数据
void prepareInfo(map<double, Concrete*>& concreteMap, map<double, Rebar*>& reinforcementMap);
///获取梁信息
void getInfo(Beam& beam);


int main(){
//	int 啊 = 1;
	map<double, Concrete*> concreteMap;//砼材料表
	map<double, Rebar*> reinforcementMap;//钢筋材料表
	prepareInfo(concreteMap, reinforcementMap);
	Beam beam;
	while(true){
		getInfo(beam);
		beam.design();
	}
	return 0;
}

void getInfo(Beam& beam){
	cout << "b h c" << endl;
	double b, h, c;//宽、高、保护层厚度
	cin >> b >> h >> c;
	Section* section = new RectSection(b, h);
	beam.setSection(section, c);
	beam.setMaterial(14.3, 2.01, 360, 360, 360);
	cout << "N V2 V3 T M2 M3" << endl;
	double n, v2, v3, t, m2, m3;
	cin >> n >> v2 >> v3 >> t >> m2 >> m3;
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

void prepareInfo(map<double, Concrete*>& concreteMap, map<double, Rebar*>& reinforcement){
	for(int i = 15; i <= 80; i +=5){
		double curName = i;
		Concrete* curConcrete = new Concrete(curName);
		concreteMap.insert(concreteMap.end(), pair<double, Concrete*>(curName, curConcrete));
	}
}