#pragma once

#include <set>
#include <map>

const double EPSILON = 1e-6;//double���ֵ

struct Force
{//����
	double N;
	double V2;
	double V3;
	double T;
	double M2;
	double M3;
};

//stage:0-�ڲ�;1-������;2-�Ҳ����;3-˫�����
double interpolation(double x1, double y1, double x2, double y2, double x, int stage);//��ֵ����
double interpolation(double x1, double y1, double x2, double y2, double x);//��ֵ����

//��ȡmapֵ����û�����½�
template<typename KeyName, class ValueName>
ValueName* getMapValueClassPt(std::map<KeyName, ValueName*>& classMap, KeyName key)
{
	if (classMap.find(key) == classMap.end()){
		ValueName* classPt = new ValueName(key);
		classMap.insert(classMap.end(), std::pair<KeyName, ValueName*>(key, classPt));
	}
	return classMap[key];
}