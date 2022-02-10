#pragma once
#include "MaterialObject.h"
class ConstructableGeometry:public OperatorModel
{
public:
	SPoint getDistance(const Vector3& pos, const Vector3& dir, DevelopmentKit& kit);
	Matrix getRot() {
		return Matrix();
	}
	Vector3 getPos() {
		return Vector3();
	}
};

