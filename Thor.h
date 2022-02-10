#pragma once
#include "MaterialObject.h"
#include "Equation.h"
class Thor :public MaterialObject
{
private:
	double rad, RAD;
public:
	Thor(double x, double y, double z, double rad, double RAD, MaterialModel* mm);
	void getNorm(Vector3& internal);
	Segments* intersectAll(const Vector3& pos, const Vector3& dir, DevelopmentKit& kit);
	SPoint getDistance(const Vector3& pos, const Vector3& dir, DevelopmentKit& kit);
private:
	inline bool simple_check(const Vector3& p, const Vector3& d) const;
	unsigned getRoots(const Vector3& pos, const Vector3& dir, double* x) const;
};

