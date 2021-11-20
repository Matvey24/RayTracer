#pragma once
#include "ObjectModel.h"
class Sphere:public ObjectModel
{
private:
	double rad2;
public:
	Sphere(double x, double y, double z, double rad, MaterialModel *mm);
	void getNorm(Vector3 &internal);
	Segments *intersectAll(const Vector3& pos, const Vector3& dir, DevelopmentKit& kit);
	SPoint getDistance(const Vector3& pos, const Vector3& dir, DevelopmentKit& kit);
};

