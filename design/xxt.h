#pragma once

const double EPSILON = 1e-6;//double误差值

//stage:0-内插;1-左侧外差;2-右侧外差;3-双侧外差
double interpolation(double x1, double y1, double x2, double y2, double x, int stage);//差值计算
double interpolation(double x1, double y1, double x2, double y2, double x);//差值计算