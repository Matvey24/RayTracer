#pragma once
#include "SPoint.h"
#include "Matrix.h"
class DevelopmentKit;
class Segments;
class OperatorModel {
public:
	virtual Segments *intersectAll(const Vector3 &p, const Vector3 &d, DevelopmentKit &kit) = 0;
	virtual SPoint getDistance(const Vector3& pos, const Vector3& dir, DevelopmentKit &kit) = 0;
	virtual void translate(const Vector3& vec) = 0;
	virtual void rotate(const Vector3& at, const Matrix& m) = 0;
	virtual Matrix getRot() = 0;
	virtual Vector3 getPos() = 0;
};