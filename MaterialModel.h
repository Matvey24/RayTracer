#pragma once
#include "Color.h"
#include "Vector3.h"
class MaterialModel {
public:
	double refraction = 0;
	Color transparency;
	virtual double reflective(const Vector3& inter) = 0;
	virtual Color reflection(const Vector3& inter) = 0;
};