#include <iostream>
#include <cmath>
//#include <assert.h>
#include "xxt.h"

double interpolation(double x1, double y1, double x2, double y2, double x, int stage)
{//stage:0-内插;1-左侧外差;2-右侧外差;3-双侧外差
	//[]有什么办法能直接中断程序抛出错误？
  //[XXT]double的比较需要特殊处理
  if(abs(x1 - x2) <= EPSILON) return 999;//{ASSERT(0);cerr << "内插值x1、x2不能相等！" << endl;}
  switch(stage)
  {
  case 0:
    return interpolation(x1, y1, x2, y2, x);
    break;
  case 1:
    if(x < x1 - EPSILON) return y1 + (y2 - y1) / (x2 - x1) * (x - x1);
    return interpolation(x1, y1, x2, y2, x);
    break;
  case 2:
    if(x > x2 + EPSILON) return y1 + (y2 - y1) / (x2 - x1) * (x - x1);
    return interpolation(x1, y1, x2, y2, x);
    break;
  case 3:
    return y1 + (y2 - y1) / (x2 - x1) * (x - x1);
    break;
  default:
    break;
  }
}

double interpolation(double x1, double y1, double x2, double y2, double x)
{
	//[]有什么办法能直接中断程序抛出错误？
	if(abs(x1 - x2) <= EPSILON) return 999;//{ASSERT(0);cerr << "内插值x1、x2不能相等！" << endl;}
	if(x < x1 + EPSILON){
		return y1;
	}else if(x > x1 - EPSILON){
		return y2;
	}else{
		return y1 + (y2 - y1) / (x2 - x1) * (x - x1);
	}
}