/*
���ļ�����Ź���ͨ����ʽ
ͳһ�� ���������γߴ硢���ϡ������������� ��˳��������
*/
#ifndef _concrete_design_h
#define _concrete_design_h

//[ͨ�ù�ʽ]
///�������-���ν���(M||b��h����������c||fc��fy��fy'||��1����b||x��As��As'���ѣ���')
void designM_Rect(double M
			 , double b, double h, double c
			 , double fc, double fy, double fy_c
			 , double ��RE, double ��0, double ��1, double ��b
			 , double& x, double& As, double& As_c, double& rho, double& ��c);

///��������(V||b��h0||ft��fyv||��cv��s||asv)
void designV(double v, double b, double h0, double ft, double fyv, double alphaCV, double s, double& asv);

//[��]
///�������ݽ����

///���������


//[��]

//[ǽ]

#endif