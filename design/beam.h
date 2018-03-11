#pragma once
//构件类-梁类
#include <vector>
#include <map>
#include "section.h"
#include "material.h"
#include "force_effect.h"
#include "xxt.h"

class ConcreteElement
{
public:
	ConcreteElement();
	virtual ~ConcreteElement();

	static void setγ0(double γ0);
	static void setCaspMap(std::map<std::string, E_SingleCaseType>* caseMap);//设置单工况类型表
	static void setFactorFC_Str(std::vector<std::string>* factorFC);//设置基本组合系数表(string形式)
	static void setFactorNC_Str(std::vector<std::string>* factorNC);//设置标准组合系数表(string形式)
	static void setFactorQPC_Str(std::vector<std::string>* factorQMC);//设置准永久组合系数表(string形式)
	static void printAllFactorTables();//输出所有组合

protected:
	static double γ0;//结构安全等级系数//[XXT]静态成员变量必须在所在类型声明前在外部进行声明
	//字符串形式的组合系数表
	//typedef std::vector<std::pair<double, std::string>> TD_CombinationExp;//[]以后利用typedef缩减代码
	static std::map<std::string, E_SingleCaseType>* m_caseMap;//单工况类型表
	static std::vector<std::string>* m_FactorFC_Str;//基本组合系数表(string形式)
	static std::vector<std::string>* m_FactorNC_Str;//标准组合系数表(string形式)
	static std::vector<std::string>* m_FactorQPC_Str;//准永久组合系数表(string形式)
	//解析好后的组合系数表
	static std::vector<CombineData> m_FactorFC;//基本组合系数表(解析后)
	static std::vector<CombineData> m_FactorNC;//标准组合系数表(解析后)
	static std::vector<CombineData> m_FactorQPC;//准永久组合系数表(解析后)
private:
	static void parserFactorFC();//解析基本组合系数表
	static void parserFactorNC();//解析标准组合系数表
	static void parserFactorQPC();//解析准永久组合系数表
private://次要内部函数
	static void parserCombination//根据 荷载组合系数表(string形式)、单工况类型表 生成对应的荷载组合系数表(解析后)
		(std::vector<std::string>* factorTable_Str
		, std::map<std::string, E_SingleCaseType>*& caseMap
		, std::vector<CombineData>& factorTable
		, E_CombinationType combineType);
	static void parserCombinationExp//解析荷载组合表达式
		(const std::string& line
		, std::vector<CombineData>& factorTable
		, E_CombinationType combineType);
	static E_CombinationType getCombinationType//分析当前荷载组合类型
		(E_CombinationType combineType
		, E_SingleCaseType maxSingleCaseType);
	static void printFactorTable(
		const std::vector<CombineData>& factorTable);
};

enum E_BeamType//[]以后把这个放到Beam类中
{
	E_BT_FRAME_BEAM = 0,//框架梁
	E_BT_BEAM,//非框架梁
	E_BT_COUPLING_BEAM,//连梁
	E_BT_TRANSFER_BEAM//转换梁
};

enum E_BeamSectionLocation//[]以后把这个放到Beam类中
{
	E_BSL_NULL,//初始化截面
	E_BSL_LR,//支座截面
	E_BSL_M//中间截面
};

class BeamSection;

class Beam : public ConcreteElement
{
	const int SECTION_NUMBER = 9;//验算截面个数
public:
	Beam();
	virtual ~Beam();

	void setBeamType(E_BeamType beamType);//设置梁类型
	void setSection(const std::vector<Section*>& sections, double c, double Lc2, double Lc3);//设置几何参数
	void setMaterial(Concrete* concrete, Rebar* rebarL, Rebar* rebarS, Steel* steel);//设置材料
	void setCalculateParameter(E_NFB Nfb, E_NFB Nfb_gz);//设置计算参数
	void setCaseMap(const std::vector<std::map<std::string, CaseData>>& caseMaps);//设置各截面单工况内力
	void calcForceData();//计算内力组合
	void showResult();//展示结果

private:
	E_BeamType beamType;//梁类型
	//几何参数
	double c;//保护层厚度
	double Lc2;//局部坐标轴2方向上的计算长度
	double Lc3;//局部坐标走3方向上的计算长度
	//材料参数
	Concrete* concrete;//砼材料
	Rebar* longitudinal;//纵筋材料
	Rebar* stirrup;//箍筋材料
	Steel* skeleton;//钢骨材料
	//计算参数
	E_NFB Nfb;//抗震等级，0-特一级/1-一级/2-二级/3-三级/4-四级/5-非抗震
	E_NFB Nfb_gz;//抗震构造等级，0-特一级/1-一级/2-二级/3-三级/4-四级/5-非抗震

	std::vector<BeamSection> sections;//梁截面数据

	friend class DesignBeam;
};

//*------------------------------------*//

class BeamSection
{//包含梁设计断面的所有数据
public:
	BeamSection();
	~BeamSection();

	void setSectionLocationType(E_BeamSectionLocation sectionLocation);
	void setSection(Section* section);//设置几何参数
	void setForceData//设置内力数据
		(const std::map<std::string, CaseData>& caseMap
		, std::vector<CombineData>* factorFC
		, std::vector<CombineData>* factorNC
		, std::vector<CombineData>* factorQPC
		);
	void calcForceData();//计算内力组合
	void showResult();//展示结果

private://保存梁基本参数、内力、计算结果
	E_BeamSectionLocation sectionLocation;
	//几何参数
	Section* section;//截面参数
	//内力参数
	ForceEffect forceData;//内力数据
	//设计结果
	struct ResultFC
	{//基本组合计算结果-配筋
		//ULS结果
		double x;//受压区高度mm
		double As;//受拉钢筋面积mm²
		double As_c;//受压钢筋面积mm²
		double ρ;//受拉钢筋配筋率ρ
		double ρc;//受压钢筋配筋率ρ'
		double Asv;//箍筋面积mm²
		double ρsv;//箍筋面积率ρsv
	};
	std::vector<ResultFC> m_resultFC;//各断面基本组合设计结果

	struct ResultNC
	{//标准组合计算结果-挠度、裂缝(预应力砼)
		//SLS结果
		double δ;//挠度mm
		double ω;//裂缝宽度mm
	};
	std::vector<ResultNC> m_resultNC;//各断面标准组合设计结果

	struct ResultQPC
	{//准永久组合计算结果-挠度、裂缝(钢筋砼)
		//SLS结果
		double δ;//挠度mm
		double ω;//裂缝宽度mm
	};
	std::vector<ResultQPC> m_resultQPC;//各断面基本组合设计结果

	friend class DesignBeam;
};