#pragma once
#include "ComplexModel.h"
#include "Intersection.h"
#include "HalfVolume.h"
class Tetrahedron:public ComplexModel
{
public:
	HalfVolume d, l, r, b;
	Intersection in;
	Tetrahedron(double x, double y, double z, double s, MaterialModel* mm);
};

