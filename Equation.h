#pragma once
#ifndef EQUATION_SOLVER_
#define EQUATION_SOLVER_
#include<math.h>
class Equation
{
public:
	unsigned root4f(double a, double b, double c, double d, double e, double* x) const;
	unsigned root4s(double p, double q, double r, double* x) const;
	unsigned root4(double a, double b, double c, double d, double* x) const;
	unsigned root3(double b, double c, double d, double* x) const;
	unsigned root3s(double c, double d, double* x) const;
	unsigned root2(double b, double c, double* x) const;
};
#endif
