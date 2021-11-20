#pragma once
#include "SurfacePoint.h"
#include "SPoint.h"
class DevelopmentKit;
class Segments;
class OperatorModel {
public:
	int type = 0;
	virtual SurfacePoint intersect(const Vector3& pos, const Vector3& dir, DevelopmentKit &kit) = 0;
	virtual Segments *intersectAll(const Vector3 &p, const Vector3 &d, DevelopmentKit &kit) = 0;
	virtual SPoint getDistance(const Vector3& pos, const Vector3& dir, DevelopmentKit &kit) = 0;
	virtual void translate(const Vector3& vec) = 0;
	virtual void rotate(const Vector3& at, const Matrix& m) = 0;
};