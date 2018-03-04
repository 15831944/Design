#include <iostream>
#include <cmath>
//#include <assert.h>
#include "xxt.h"

double interpolation(double x1, double y1, double x2, double y2, double x, int stage)
{//stage:0-�ڲ�;1-������;2-�Ҳ����;3-˫�����
	//[]��ʲô�취��ֱ���жϳ����׳�����
  //[XXT]double�ıȽ���Ҫ���⴦��
  if(abs(x1 - x2) <= EPSILON) return 999;//{ASSERT(0);cerr << "�ڲ�ֵx1��x2������ȣ�" << endl;}
  switch(stage)
  {
  case 0:
    x = x > x2 ? x2 : x;
    x = x < x1 ? x1 : x;
    break;
  case 1:
    x = x > x2 ? x2 : x;
    break;
  case 2:
    x = x < x1 ? x1 : x;
    break;
  case 3:
    break;
  default:
    break;
  }
  return interpolation(x1, y1, x2, y2, x);
}

double interpolation(double x1, double y1, double x2, double y2, double x)
{
	//[]��ʲô�취��ֱ���жϳ����׳�����
	if(abs(x1 - x2) <= EPSILON) return 999;//{ASSERT(0);cerr << "�ڲ�ֵx1��x2������ȣ�" << endl;}
	return y1 + (y2 - y1) / (x2 - x1) * (x - x1);
}