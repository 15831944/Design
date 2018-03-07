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
///׼������
void prepareInfo
	(std::map<double, Concrete*>& concreteMap
	, std::map<double, Rebar*>& rebarMap
	, std::map<double, Steel*>& steelMap
	, std::vector<std::string>& factorFC
	, std::vector<std::string>& factorNC
	, std::vector<std::string>& factorQPC
	);
///��ȡ����Ϣ
void getInfo
	(Beam& beam
	, std::map<double, Concrete*>& concreteMap
	, std::map<double, Rebar*>& rebarMap
	, std::map<double, Steel*>& steelMap
	, std::set<Section*>& sectionSet
	);

int main(){
//	test();
	std::set<Section*> sectionSet;//�����//[]�Ժ��ǵ���map��λ���
	std::map<double, Concrete*> concreteMap;//�Ų��ϱ�
	std::map<double, Rebar*> rebarMap;//�ֽ���ϱ�
	std::map<double, Steel*> steelMap;//�ֲı�
	std::vector<std::string> factorFC;//�������ϵ����
	std::vector<std::string> factorNC;//��׼���ϵ����
	std::vector<std::string> factorQPC;//׼�������ϵ����

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
	//��ʼ���������ϵ����
	factorFC.insert(factorFC.end(), "1.35D+0.98L");
	factorFC.insert(factorFC.end(), "1.2D+1.4L");
	factorFC.insert(factorFC.end(), "1D+1.2L");
	//��ʼ����׼���ϵ����
	factorNC.insert(factorNC.end(), "1D+1L");
	//��ʼ��׼�������ϵ����
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
	/*-----����Ϊ��ʱ��������-----*/

	/*
	std::cout << "��ȫϵ��������ȼ���������ȼ�" << std::endl;
	double ��0;
	int Nfb, Nfb_gz;
	std::cin >> ��0 >> Nfb >> Nfb_gz;
	beam.setCalculateParameter(��0, Nfb, Nfb_gz);

	std::cout << "�����ͣ�0-�������1-�ǿ������2-������3-ת����" << std::endl;
	int beamType;
	std::cin >> beamType;
	beam.setBeamType((E_BeamType)beamType);//[]����ǵ�д��ǿת������಻����

	std::cout << "���ߡ���������" << std::endl;
	double b, h, c;//���ߡ���������
	std::cin >> b >> h >> c;
	Section* section = new RectSection(b, h);
	beam.setSection(section, c);
	
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