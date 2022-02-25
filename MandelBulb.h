#pragma once
#include "Sphere.h"
class MandelBulb:public MaterialObject
{
private:
	Sphere sp;
	double rad;
	const size_t fract_iter = 30;
	const double power = 8;
	const double Bailout = 2;
	const double diff = 0.001;
	const double minimum_distance = 0.001;
	const size_t raymarch_steps = 200;
public:
	MandelBulb(double x, double y, double z, double rad, MaterialModel* mm);
	void getNorm(Vector3& internal);
	Segments* intersectAll(const Vector3& pos, const Vector3& dir, DevelopmentKit& kit);
	SPoint getDistance(const Vector3& pos, const Vector3& dir, DevelopmentKit& kit);
	double DE(Vector3& internal);
};