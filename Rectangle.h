#pragma once
#include "MaterialObject.h"
class Rectangle: public MaterialObject
{
public:
	double w, h, d;
	Rectangle(double x, double y, double z, double w, double h, double d, MaterialModel* mm);
	void getNorm(Vector3& internal);
	Segments* intersectAll(const Vector3& pos, const Vector3& dir, DevelopmentKit& kit);
	SPoint getDistance(const Vector3& pos, const Vector3& dir, DevelopmentKit& kit);
};

