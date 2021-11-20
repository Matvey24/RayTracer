#pragma once
#include "ObjectModel.h"
class HalfVolume:public ObjectModel
{
	
public:
	HalfVolume(double x, double y, double z, MaterialModel* mm);
	void getNorm(Vector3& internal);
	Segments* intersectAll(const Vector3& pos, const Vector3& dir, DevelopmentKit& kit);
	SPoint getDistance(const Vector3& pos, const Vector3& dir, DevelopmentKit& kit);
};


