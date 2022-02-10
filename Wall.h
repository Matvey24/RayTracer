#pragma once
#include "MaterialObject.h"
class Wall:public MaterialObject
{
private:
	double hwidth;
public:
	Wall(double x, double y, double z, double hwid, MaterialModel* mm);
	void getNorm(Vector3& internal);
	Segments* intersectAll(const Vector3& pos, const Vector3& dir, DevelopmentKit& kit);
	SPoint getDistance(const Vector3& pos, const Vector3& dir, DevelopmentKit& kit);
};

