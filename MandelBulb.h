#pragma once
#include "Sphere.h"
class MandelBulb:public MaterialObject
{
private:
	Sphere sp;
	const size_t fract_iter = 50;
	const double power = 8;
	const double Bailout = 2;
	const double diff = 0.001;
	const double minimum_distance = 0.0001;
	const size_t raymarch_steps = 500;
public:
	MandelBulb(double x, double y, double z, MaterialModel* mm);
	void getNorm(Vector3& internal);
	Segments* intersectAll(const Vector3& pos, const Vector3& dir, DevelopmentKit& kit);
	SPoint getDistance(const Vector3& pos, const Vector3& dir, DevelopmentKit& kit);
	double DE(Vector3& internal);
};