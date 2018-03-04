#include <iostream>
#include <sstream>
#include <fstream>
#include <string>

#include <map>

#include "beam.h"
#include "Section.h"
#include "material.h"
#include "xxt.h"

//[portotype]
///׼������
void prepareInfo(std::map<double, Concrete*>& concreteMap, std::map<double, Rebar*>& rebarMap);
///��ȡ����Ϣ
void getInfo(Beam& beam, std::map<double, Concrete*>& concreteMap, std::map<double, Rebar*>& rebarMap);


int main(){
	std::map<double, Concrete*> concreteMap;//�Ų��ϱ�
	std::map<double, Rebar*> rebarMap;//�ֽ���ϱ�
	prepareInfo(concreteMap, rebarMap);
	Beam beam;
	while(true){
		getInfo(beam, concreteMap, rebarMap);
		beam.design();
	}
	return 0;
}

void getInfo(Beam& beam, std::map<double, Concrete*>& concreteMap, std::map<double, Rebar*>& rebarMap){
	std::cout << "b h c" << std::endl;
	double b, h, c;//�����ߡ���������
	std::cin >> b >> h >> c;
	Section* section = new RectSection(b, h);
	beam.setSection(section, c);
	
	std::cout << "�š��ݽ����ֹǲ��ϵȼ�" << std::endl;
	double concreteName, rebarL, rebarS, skeleton;//�š��ݽ����ֹǲ���
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

void prepareInfo(std::map<double, Concrete*>& concreteMap, std::map<double, Rebar*>& reinforcement){
	for(int i = 15; i <= 80; i +=5){
		double curName = i;
		Concrete* curConcrete = new Concrete(curName);
		concreteMap.insert(concreteMap.end(), std::pair<double, Concrete*>(curName, curConcrete));
	}
}