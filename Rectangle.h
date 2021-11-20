#pragma once
#include "ComplexModel.h"
#include "Wall.h"
#include "Intersection.h"
class Rectangle:public ComplexModel
{
public:
	Wall ud, lr, fb;
	Intersection in;
	Rectangle(double x, double y, double z, double w, double h, double d, MaterialModel* mm);
};

