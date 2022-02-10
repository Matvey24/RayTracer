#pragma once
#include "SurfacePoint.h"
#include "OperatorModel.h"
#include "DevelopmentKit.h"
class ObjectModel : public OperatorModel {
public:
	virtual SurfacePoint getPoint(const Vector3& pos, const Vector3& dir, double dist, DevelopmentKit& kit) = 0;
};