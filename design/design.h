#pragma once
//����࣬��ȡ������������
#include "xxt.h"
#include "beam.h"

class Design
{
public:
	Design();
	virtual ~Design();

	virtual void setData(void* data) = 0;//��������
	virtual void design() = 0;//�������
protected:

};

//*------------------------------------*//

class DesignBeam : public Design
{
public:
	DesignBeam(){}
	explicit DesignBeam(Beam* beam);
	virtual ~DesignBeam();
	virtual void setData(void* data);//��������
	virtual void design();//�������additionData
private:
	struct AdditionData
	{//��������
		//�ǵ���������
		double ��b;//����ѹ����ֵ
		double ��min;//��С�����
		double ��max;//��������
		double ��min_sv;//��С�乿��
		//�˷�������
		double ��Limit;//������ֵ
		double ��b_AD;
		double ��min_AD;
		double ��max_AD;
		double ��min_sv_AD;
		//����������
		double ��b_E;
		double ��RE_M;//�������RE
		double ��RE_V;//б�����RE
		double ��min_E_LR;//֧����min
		double ��min_E_M;//���Ц�min
		double ��max_E;
		double ��min_sv_E;
		double minAs_cRatio_LR;//������������������С��ֵ
		double minAsRatioContinue;//��ͨ�ֽ���������ı�ֵ
	} additionData;//�൱�����ݿ�
	//һ�¼�������Ϊ�����ͨ�����ݣ�����������ϵ�����ȡ��Ӧֵ��ͳһ����designM��designV�������
	double ��b;//����ѹ����ֵ
	double ��RE;
	double ��min;//��С�����
	double ��max;//��������
	double ��min_sv;//��С�乿��
	//����ƵĹ���
	Beam* beam;

//[]��Щ������ô��д��virtual��
	void prepare();//׼������
	void setParameter();//���ɷǵ�����������ز���
	void setParameterAD();//�����˷���������ز���
	void setParameterE();//���ɵ�����������ز���

	void designULS();//������������״̬���
	void designSLS();//����ʹ�ü���״̬���
	void designSection
		(Section* section
		, E_BeamSectionLocation sectionType
		, const ForceData& forceData
		, BeamSection::ResultFC& resultFC);//��һ�������
	void setDesignParameterN(E_BeamSectionLocation sectionType);//���÷ǵ�����Ͻ�����Ʋ���
	void setDesignParameterAD(E_BeamSectionLocation sectionType);//�����˷���Ͻ�����Ʋ���
	void setDesignParameterE(E_BeamSectionLocation sectionType);//���õ�����Ͻ�����Ʋ���

	void designM(Section* section, const ForceData& forceData, BeamSection::ResultFC& resultFC);//����ݽ�
	void designV(Section* section, const ForceData& forceData, BeamSection::ResultFC& resultFC);//��ƹ���

	double calc_��min_AD();//�����min_AD
	double calc_��max_AD();//�����max_AD
};
