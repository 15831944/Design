#include "design.h"
#include "concrete_design.h"//砼构件计算公式

const double AS_SINGLE = 22.5;//单筋保护层额外厚度as=c+AS_SINGLE按10箍筋25纵筋考虑
const double AS_DUAL = 47.5;//双筋保护层额外厚度as=c+AS_DUAL按10箍筋25纵筋考虑
const double RHO_LIMIT = 0.01;//控制是否按双排配筋计算的配筋率界限，ρ<RHO_LIMIT单排，ρ≥RHO_LIMIT双排

//[portotype]
///计算ξb
double getKxiB(double beta1, double fy, double Es, double stressLimitC);

///计算配筋
static void calculateAs(double M, double b, double h, double as, double as_c, double fc, double fy, double fy_c, double alpha1, double ksiB, double rhoMin_c, double& x, double& As, double& As_c);
///计算x
static void calculateX(double M, double b, double h0, double fc, double fy, double alpha1, double& x, double& As_c);


Design::Design(){}

Design::~Design(){}

//*------------------------------------*//

DesignBeam::DesignBeam(Beam::Data* data){
	this->data = data;
}

DesignBeam::~DesignBeam(){}

void DesignBeam::setData(void* data){
	this->data = (Beam::Data*) data;
}

void DesignBeam::design(){
	prepare();
	designM();
	designV();
}

void DesignBeam::prepare(){
	//additionData.ksiB = getKxiB(additionData.beta1, data->fy, 200000, additionData.stressLimitC);//Es，εcu待求
}

void DesignBeam::designM(){
	/*
	bool isSingleRowAs = true;//标识受拉钢筋是否按单排钢筋计算
	bool isSingleRowAs_c = true;//标识受压钢筋是否按单排钢筋计算



	while(true){
		double as = data->c + (isSingleRowAs? AS_SINGLE : AS_DUAL);//as
		double as_c = data->c + (isSingleRowAs_c? AS_SINGLE : AS_DUAL);//as'
		calculateAs(M ,b ,h, as, as_c, fc, fy, fy_c, alpha1, ksiB, x, As, As_c);
		double h0 = data->section - as;

		rho = As / b / h0;//ρ
		rho_c = As_c / b / h0;//ρ'
		//判断是否需要重新计算
		bool needCal = false;//标识是否重新计算
		//判断受拉钢筋是否要重算
		needCal = ((rho < RHO_LIMIT) != isSingleRowAs) || needCal;
		if(needCal) isSingleRowAs = !isSingleRowAs;
		//判断受压钢筋是否要重算
		needCal = ((rho_c < RHO_LIMIT) != isSingleRowAs_c) || needCal;
		if(needCal) isSingleRowAs_c = !isSingleRowAs_c;
		if(!needCal) break;
	}*/
}

static void calculateAs(double Md, double as, double as_c){
	/*double h0 = h - as;
	double sqrtM = h0 * h0 - 2 * M * ForceUnit::k_kN * LengthUnit::k_m / (alpha1 * fc * b);//根号里面的内容
	double sqrtLimit = h0 * h0 * (1 - ksiB) * (1 - ksiB);
	if(sqrtM < sqrtLimit){//此时x>xb，按双筋计算，As'要比ρmin大
		x = ksiB * h0;
		As_c = (M * ForceUnit::k_kN * LengthUnit::k_m - alpha1 * fc * b * x * (h0 - x / 2)) / fy_c / (h0 - as_c);

		double As_cMin = rhoMin_c * b * h;
		if(As_c < As_cMin){
			As_c = As_cMin;
			double Md = M - fy_c * As_c * (h0 - as_c) / ForceUnit::k_kN / LengthUnit::k_m;
			sqrtM = h0 * h0 - 2 * Md * ForceUnit::k_kN * LengthUnit::k_m / (alpha1 * fc * b);
		}

	}else{//此时x<xb，按单筋计算，As'取ρmin
		x = h0 - sqrt(sqrtM);
	}
	As = (alpha1 * fc * b * x + fy_c * As_c) / fy;
	if(x < 2 * as_c){
		As = M / fy / (h - as - as_c);
	}*/
}

void DesignBeam::designV(){
	
}

double getKxiB(double beta1, double fy, double Es, double stressLimitC){
	return beta1 / (1 + fy / (Es * stressLimitC));
}