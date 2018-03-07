#pragma once
//存放各类通用声明、方法
#include <set>
#include <map>

//[enum]
///力单位转换统一到N
enum E_ForceUnit
{
	E_FU_N = 1,
	E_FU_kN = 1000
};
///长度单位转换统一到mm
enum E_LengthUnit
{
	E_LU_mm = 1,//mm
	E_LU_m = 1000//m
};
///抗震等级
//[]暂时没用上
enum E_NFB
{
	E_NFB_LV0,
	E_NFB_LV1,
	E_NFB_LV2,
	E_NFB_LV3,
	E_NFB_LV4,
	E_NFB_NULL
};
///工况类型
enum E_SingleCaseType
{
	E_SCT_DEAD,//恒载
	E_SCT_LIVE,//活载
	E_SCT_WIND,//风载
	E_SCT_TEMPERATURE,//温度作用
	E_SCT_PRESTRESS,//预应力
	E_SCT_AD,//人防荷载
	E_SCT_E//地震作用
};
///组合类型
enum E_CombinationType
{
	E_CT_CASE,//单工况
	E_CT_LOAD,//非地震组合
	E_CT_SEISMIC,//地震组合
	E_CT_AD,//人防组合
	E_CT_NOMINAL,//标准组合
	E_CT_QP//准永久组合
};

//[const]
const double EPSILON = 1e-6;//double误差值

//[struct]
struct Force
{//内力
	double N;
	double V2;
	double V3;
	double T;
	double M2;
	double M3;
};

//stage:0-内插;1-左侧外差;2-右侧外差;3-双侧外差
double interpolation(double x1, double y1, double x2, double y2, double x, int stage);//差值计算
double interpolation(double x1, double y1, double x2, double y2, double x);//差值计算

//获取map值，若没有则新建
template<typename KeyName, class ValueName>
ValueName* getMapValueClassPt(std::map<KeyName, ValueName*>& classMap, KeyName key)
{
	if (classMap.find(key) == classMap.end()){
		ValueName* classPt = new ValueName(key);
		classMap.insert(classMap.end(), std::pair<KeyName, ValueName*>(key, classPt));
	}
	return classMap[key];
}