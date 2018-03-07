#pragma once
//��Ÿ���ͨ������������
#include <set>
#include <map>

//[enum]
///����λת��ͳһ��N
enum E_ForceUnit
{
	E_FU_N = 1,
	E_FU_kN = 1000
};
///���ȵ�λת��ͳһ��mm
enum E_LengthUnit
{
	E_LU_mm = 1,//mm
	E_LU_m = 1000//m
};
///����ȼ�
//[]��ʱû����
enum E_NFB
{
	E_NFB_LV0,
	E_NFB_LV1,
	E_NFB_LV2,
	E_NFB_LV3,
	E_NFB_LV4,
	E_NFB_NULL
};
///��������
enum E_SingleCaseType
{
	E_SCT_DEAD,//����
	E_SCT_LIVE,//����
	E_SCT_WIND,//����
	E_SCT_TEMPERATURE,//�¶�����
	E_SCT_PRESTRESS,//ԤӦ��
	E_SCT_AD,//�˷�����
	E_SCT_E//��������
};
///�������
enum E_CombinationType
{
	E_CT_CASE,//������
	E_CT_LOAD,//�ǵ������
	E_CT_SEISMIC,//�������
	E_CT_AD,//�˷����
	E_CT_NOMINAL,//��׼���
	E_CT_QP//׼�������
};

//[const]
const double EPSILON = 1e-6;//double���ֵ

//[struct]
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