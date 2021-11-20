#pragma once
#include "ObjectModel.h"
class ConstructableGeometry:public OperatorModel
{
public:
	SurfacePoint intersect(const Vector3& pos, const Vector3& dir, DevelopmentKit& kit);
	SPoint getDistance(const Vector3& pos, const Vector3& dir, DevelopmentKit& kit);
};

