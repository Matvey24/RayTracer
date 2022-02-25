#pragma once
#include "Color.h"
#include "Matrix.h"
#include "DevelopmentKit.h"
class MaterialModel {
public:
	bool improved = false;
	bool self_lighting = false;
	double refraction = 0;
	Color transparency;
	virtual double reflective(const Vector3& inter) = 0;
	virtual Color reflection(const Vector3& inter) = 0;
	virtual Color improvedReflection(const Vector3& inter, const Vector3& dir, DevelopmentKit& kit) = 0;
};