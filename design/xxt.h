#pragma once

#include <set>
#include <map>

const double EPSILON = 1e-6;//double误差值

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