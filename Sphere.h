#pragma once
#include "MaterialObject.h"
class Sphere:public MaterialObject
{
public:
	double rad2;
	Sphere(double x, double y, double z, double rad, MaterialModel *mm);
	void getNorm(Vector3 &internal);
	Segments *intersectAll(const Vector3& pos, const Vector3& dir, DevelopmentKit& kit);
	SPoint getDistance(const Vector3& pos, const Vector3& dir, DevelopmentKit& kit);
	bool internal(Vector3& pos);
};

