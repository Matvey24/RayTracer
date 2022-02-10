#pragma once
#include "MaterialModel.h"
class Mirror :public MaterialModel
{
public:
	double reflective(const Vector3& inter);
	Color reflection(const Vector3& inter);
	Color improvedReflection(const Vector3& inter, const Vector3& dir, DevelopmentKit& kit) { return reflection(inter); }
};