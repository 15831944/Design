/*
此文件存放砼构件配筋公式
统一按 内力、几何尺寸、材料、计算参数、验算参数、结果 的顺序传入数据
*/
#include "concrete_design.h"
#include <cmath>
#include "design.h"

using namespace std;

const double AS_SINGLE = 22.5;//单筋保护层额外厚度as=c+AS_SINGLE按10箍筋25纵筋考虑
const double AS_DUAL = 47.5;//双筋保护层额外厚度as=c+AS_DUAL按10箍筋25纵筋考虑
const double RHO_LIMIT = 0.01;//控制是否按双排配筋计算的配筋率界限，ρ<RHO_LIMIT单排，ρ≥RHO_LIMIT双排


//[portotype]
///计算配筋
static void calculateAs(double M, double b, double h, double as, double as_c, double fc, double fy, double fy_c, double alpha1, double ksiB, double rhoMin_c, double& x, double& As, double& As_c);
///计算x
static void calculateX(double M, double b, double h0, double fc, double fy, double alpha1, double& x, double& As_c);



//[通用公式]
void designM(double M
			 , double b, double h, double c
			 , double fc, double fy, double fy_c
			 , double gamaRE, double gama0, double alpha1, double ksiB
			 , double rhoMin_c
			 , double& x, double& As, double& As_c, double& ρ, double& ρc){
	double as = c + AS_SINGLE;//as
	double as_c = c + AS_SINGLE;//as'
	double h0 = h - as;
	As_c = rhoMin_c * b * h;
	double Md = M - fy_c * As_c * (h0 - as_c) / E_ForceUnit::E_FU_kN / E_LengthUnit::E_LU_m;
	calculateAs(Md ,b ,h, as, as_c, fc, fy, fy_c, alpha1, ksiB, rhoMin_c, x, As, As_c);
	ρ = As / b / h0;//ρ
	ρc = As_c / b / h0;//ρ'
	if(ρ < RHO_LIMIT && ρc < RHO_LIMIT) return;
	if(ρ >= RHO_LIMIT) as = c + AS_DUAL;
	if(ρc >= RHO_LIMIT) as_c = c + AS_DUAL;
//	As_c = rhoMin_c * b * h;
//	Md = M - fy_c * As_c * (h0 - as_c) / E_ForceUnit::E_FU_kN / E_LengthUnit::E_LU_m;
	calculateAs(Md ,b ,h, as, as_c, fc, fy, fy_c, alpha1, ksiB, rhoMin_c, x, As, As_c);
	ρc = As_c / b / h0;//ρ'
	if(ρc < RHO_LIMIT){
		return;
	}else{
		as_c = c + AS_DUAL;
//		As_c = rhoMin_c * b * h;
//		Md = M - fy_c * As_c * (h0 - as_c) / E_ForceUnit::E_FU_kN / E_LengthUnit::E_LU_m;
		calculateAs(Md ,b ,h, as, as_c, fc, fy, fy_c, alpha1, ksiB, rhoMin_c, x, As, As_c);
	}
}

static void calculateAs(double M, double b, double h, double as, double as_c, double fc, double fy, double fy_c, double alpha1, double ksiB, double rhoMin_c, double& x, double& As, double& As_c){
	double h0 = h - as;
	double sqrtM = h0 * h0 - 2 * M * E_ForceUnit::E_FU_kN * E_LengthUnit::E_LU_m / (alpha1 * fc * b);//根号里面的内容
	double sqrtLimit = h0 * h0 * (1 - ksiB) * (1 - ksiB);
	if(sqrtM < sqrtLimit){//此时x>xb，按双筋计算，As'要比ρmin大
		x = ksiB * h0;
		As_c = (M * E_ForceUnit::E_FU_kN * E_LengthUnit::E_LU_m - alpha1 * fc * b * x * (h0 - x / 2)) / fy_c / (h0 - as_c);
/*		double As_cMin = rhoMin_c * b * h;
		if(As_c < As_cMin){
			As_c = As_cMin;
			double Md = M - fy_c * As_c * (h0 - as_c) / E_ForceUnit::E_FU_kN / E_LengthUnit::E_LU_m;
			sqrtM = h0 * h0 - 2 * Md * E_ForceUnit::E_FU_kN * E_LengthUnit::E_LU_m / (alpha1 * fc * b);
		}*/
	}else{//此时x<xb，按单筋计算，As'取ρmin
		x = h0 - sqrt(sqrtM);
	}
	As = (alpha1 * fc * b * x + fy_c * As_c) / fy;
	if(x < 2 * as_c){
		As = M / fy / (h - as - as_c);
	}
}



/*
void beam_CalculateTransverseRebar(double v, double b, double h0, double ft, double fyv, double alphaCV, double s, double& asv){
	double vDesign = v * UNIT_TRANSFER_FACTOR_KN_TO_N -alphaCV * ft * b * h0;
	asv = vDesign * s / fyv / h0;
}
*/
//[梁]

//[柱]

//[墙]























/*
void designM(double M, double b, double h, double c, double fc, double fy, double fy_c, double alpha1, double ksiB, double rhoMin_c, double& x, double& As, double& As_c, double& rho, double& ρc){
	bool isSingleRowAs = true;//标识受拉钢筋是否按单排钢筋计算
	bool isSingleRowAs_c = true;//标识受压钢筋是否按单排钢筋计算
	while(true){
		double as = c + (isSingleRowAs? AS_SINGLE : AS_DUAL);//as
		double as_c = c + (isSingleRowAs_c? AS_SINGLE : AS_DUAL);//as'
		calculateAs(M ,b ,h, as, as_c, fc, fy, fy_c, alpha1, ksiB, x, As, As_c);
		double h0 = h - as;
		ρ = As / b / h0;//ρ
		ρc = As_c / b / h0;//ρ'
		//判断是否需要重新计算
		bool needCal = false;//标识是否重新计算
		//判断受拉钢筋是否要重算
		needCal = ((ρ < RHO_LIMIT) != isSingleRowAs) || needCal;
		if(needCal) isSingleRowAs = !isSingleRowAs;
		//判断受压钢筋是否要重算
		needCal = ((ρc < RHO_LIMIT) != isSingleRowAs_c) || needCal;
		if(needCal) isSingleRowAs_c = !isSingleRowAs_c;
		if(!needCal) break;
	}
}*/