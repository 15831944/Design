/*
���ļ�����Ź�����ʽ
ͳһ�� ���������γߴ硢���ϡ���������������������� ��˳��������
*/
#ifndef _concrete_design_h
#define _concrete_design_h

//[ͨ�ù�ʽ]
///�������(M||b��h����������c||fc��fy��fy'||��1����b||��min||x��As��As'���ѣ���')
void designM(double M
			 , double b, double h, double c
			 , double fc, double fy, double fy_c
			 , double gamaRE, double gama0, double alpha1, double ksiB
			 , double rhoMin
			 , double& x, double& As, double& As_c, double& rho, double& rho_c);

///��������(V||b��h0||ft��fyv||��cv��s||asv)
void designV(double v, double b, double h0, double ft, double fyv, double alphaCV, double s, double& asv);

//[��]
///�������ݽ����

///���������


//[��]

//[ǽ]

#endif