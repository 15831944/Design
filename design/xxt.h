#pragma once

const double EPSILON = 1e-6;//double���ֵ

//stage:0-�ڲ�;1-������;2-�Ҳ����;3-˫�����
double interpolation(double x1, double y1, double x2, double y2, double x, int stage);//��ֵ����
double interpolation(double x1, double y1, double x2, double y2, double x);//��ֵ����