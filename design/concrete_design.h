/*
此文件存放砼构件配筋公式
统一按 内力、几何尺寸、材料、计算参数、验算参数、结果 的顺序传入数据
*/
#ifndef _concrete_design_h
#define _concrete_design_h

//[通用公式]
///纯弯计算(M||b，h，保护层厚度c||fc，fy，fy'||α1，ξb||ρmin||x，As，As'，ρ，ρ')
void designM(double M
			 , double b, double h, double c
			 , double fc, double fy, double fy_c
			 , double gamaRE, double gama0, double alpha1, double ksiB
			 , double rhoMin
			 , double& x, double& As, double& As_c, double& rho, double& rho_c);

///纯剪计算(V||b，h0||ft，fyv||αcv，s||asv)
void designV(double v, double b, double h0, double ft, double fyv, double alphaCV, double s, double& asv);

//[梁]
///矩形梁纵筋计算

///梁箍筋计算


//[柱]

//[墙]

#endif