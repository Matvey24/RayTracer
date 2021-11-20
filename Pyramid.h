#pragma once
#include "ComplexModel.h"
#include "Intersection.h"
#include "HalfVolume.h"
class Pyramid:public ComplexModel
{
public:
	HalfVolume d, l, r, f, b;
	Intersection in;
	Pyramid(double x, double y, double z, double w, double h, double d, MaterialModel* mm);
};

