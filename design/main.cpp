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
///学习数据库
void test();
///准备数据
void prepareTables
	(std::map<double, Concrete*>& concreteMap
	, std::map<double, Rebar*>& rebarMap
	, std::map<double, Steel*>& steelMap
	, std::map<std::string, ForceEffect::E_SingleCaseType>& caseMap
	, std::vector<std::string>& factorFC
	, std::vector<std::string>& factorNC
	, std::vector<std::string>& factorQPC);
///设置砼设计基础信息
void setBasicDesignInfo
	(double γ0
	, std::map<std::string, ForceEffect::E_SingleCaseType> caseMap
	, std::vector<std::string> factorFC
	, std::vector<std::string> factorNC
	, std::vector<std::string> factorQPC);
///获取梁信息
void getBeamInfo
	(Beam& beam
	, std::map<double, Concrete*>& concreteMap
	, std::map<double, Rebar*>& rebarMap
	, std::map<double, Steel*>& steelMap
	, std::set<Section*>& sectionSet);

//[静态成员变量声明]
double ConcreteElement::γ0;
std::map<std::string, ForceEffect::E_SingleCaseType>* ConcreteElement::m_caseMap;
std::vector<std::string>* ConcreteElement::m_FactorFC_Str;
std::vector<std::string>* ConcreteElement::m_FactorNC_Str;
std::vector<std::string>* ConcreteElement::m_FactorQPC_Str;
std::vector<ForceEffect::CombineExp> ConcreteElement::m_FactorFC;
std::vector<ForceEffect::CombineExp> ConcreteElement::m_FactorNC;
std::vector<ForceEffect::CombineExp> ConcreteElement::m_FactorQPC;

int main(){
//	test();
	//截面
	std::set<Section*> sectionSet;//截面表//[]以后还是得用map对位编号
	//材料
	std::map<double, Concrete*> concreteMap;//砼材料表
	std::map<double, Rebar*> rebarMap;//钢筋材料表
	std::map<double, Steel*> steelMap;//钢材表
	//荷载
	std::map<std::string, ForceEffect::E_SingleCaseType> caseMap;//单工况类型表
	std::vector<std::string> factorFC;//基本组合系数表
	std::vector<std::string> factorNC;//标准组合系数表
	std::vector<std::string> factorQPC;//准永久组合系数表

	prepareTables(concreteMap, rebarMap, steelMap, caseMap, factorFC, factorNC, factorQPC);
	std::cout << "安全系数=";
	double γ0 = 1;
	std::cin >> γ0;
	setBasicDesignInfo(γ0, caseMap, factorFC, factorNC, factorQPC);
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
	//初始化单工况类型表//[]此处待与YJK统一
	//[]这里能直接使用ConcreteElement::caseMap.insert(...)来操作吗？
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
	caseMap.insert(caseMap.end(), std::pair<std::string, ForceEffect::E_SingleCaseType>("EXY", ForceEffect::E_SingleCaseType::E_SCT_E));//X为主的双向地震作用
	caseMap.insert(caseMap.end(), std::pair<std::string, ForceEffect::E_SingleCaseType>("EYX", ForceEffect::E_SingleCaseType::E_SCT_E));//Y为主的双向地震作用
	//初始化基本组合系数表
	//注意保证单工况名称与这里的名称能对上，否则单工况内力取0
	factorFC.insert(factorFC.end(), "1.35D+0.98L");
	factorFC.insert(factorFC.end(), "1.2D+0.6L+1.3EX");
	factorFC.insert(factorFC.end(), "1D+1AD");
	//初始化标准组合系数表
	factorNC.insert(factorNC.end(), "1D+1L");
	//初始化准永久组合系数表
	factorQPC.insert(factorQPC.end(), "1D+0.6L");
}

void setBasicDesignInfo
(double γ0
, std::map<std::string, ForceEffect::E_SingleCaseType> caseMap
, std::vector<std::string> factorFC
, std::vector<std::string> factorNC
, std::vector<std::string> factorQPC)
{
	ConcreteElement::setγ0(γ0);
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
	//beam.setSection(sections, sectionLocationTypes, 20, 6000, 3000);//[?]系统不认class BeamSection的前置声明？

	Concrete* concretePt = getMapValueClassPt(concreteMap, 30.0);
	Rebar* rebarLPt = getMapValueClassPt(rebarMap, 400.0);
	Rebar* rebarSPt = getMapValueClassPt(rebarMap, 400.0);
	Steel* steelPt = getMapValueClassPt(steelMap, 235.0);
	beam.setMaterial(concretePt, rebarLPt, rebarSPt, steelPt);

	//此阶段输入梁各截面单工况内力，Beam->BeamSection->ForceEffect中的m_caseMap将放一个完整的拷贝
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
	/*-----以上为临时测试内容-----*/
}

void test()
{
	DataBase memoryDataBase;
	memoryDataBase.open(true);
	memoryDataBase.setShowLog(false);

	std::string sTableName = "AddressList";
//创建表格
	memoryDataBase.deleteTable(sTableName);
	std::vector<std::string> columnNameAndTypes = { "Name TEXT", "Age INTEGER", "Address TEXT" };
	memoryDataBase.createTable(sTableName, columnNameAndTypes);
	//std::vector<std::string> primaryKey = { "Name TEXT" };
	//memoryDataBase.createTable(sTableName, columnNameAndTypes, primaryKey);//主键不能重复
//添加数据
	int num = 1000000;
	int persentNum = 100;//百分比份数
	std::cout << "准备写入" << num * 2 << "条数据" << std::endl;
	std::vector<std::string> columnValues = { "'Jerry'", "30", "'Beijing Chaoyang'" };
	std::vector<std::string> columnNames = { "Name", "Age", "Address" };
	memoryDataBase.addRow(sTableName, columnValues, columnNames);
	for (int i = 0; i < num; i++)
	{
		columnValues = { "'Tom'", "25", "'Shanghai China'" };
		memoryDataBase.addRow(sTableName, columnValues);

		columnValues = { "'Lucy'" };
		columnNames = { "Name" };
		memoryDataBase.addRow(sTableName, columnValues, columnNames);
		if (i % (num / persentNum) == 0) std::cout << i / (num / persentNum) * 100 / persentNum << "%   ";
	}
	std::cout << num * 2 << "条数据写入成功" << std::endl;
//	memoryDataBase.printTable(sTableName);
//修改列数据
	std::string condition = "Age BETWEEN 27 AND 30";
	std::map<std::string, std::string> columnNameValuePairs = { { "Name", "'Jerry'" }, { "Age", "16" } };
	memoryDataBase.setRow(sTableName, columnNameValuePairs, condition);

//	memoryDataBase.printTable(sTableName);
//选择数据
	columnNames = { "Name", "Age" };
	condition = "Age BETWEEN 15 AND 25";
	memoryDataBase.selectColumn(sTableName, columnNames, condition);
//	memoryDataBase.printSelectResult();

//将in-memory数据库拷贝到文件数据库中
	std::cout << "开始将内存数据库写入到文件数据库" << std::endl;
	DataBase dataBase;
	std::string path = ".\\DataTest.db";
	dataBase.setPath(path);
	dataBase.setShowLog(false);
	dataBase.open(false);
	memoryDataBase.copyTo(dataBase);
	dataBase.close();
	std::cout << "写入成功" << std::endl;

	std::cout << "开始将文件数据库写入到内存数据库" << std::endl;
	DataBase inMemoryDataBase;
	dataBase.open();
	inMemoryDataBase.open(true);
	dataBase.copyTo(inMemoryDataBase);
	std::cout << "写入成功" << std::endl;


/*
//删除行数据
	condition = "Name = 'Jerry'";
	memoryDataBase.deleteRow(sTableName, condition);
*/
//删除表格
	memoryDataBase.deleteTable(sTableName);
	memoryDataBase.close(); 
}