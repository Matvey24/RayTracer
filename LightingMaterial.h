#pragma once
#include "MaterialModel.h"
class LightingMaterial :public MaterialModel
{
public:
	Color main_c;
	LightingMaterial(unsigned color, double pow);
	LightingMaterial(Color& c, double pow);
	double reflective(const Vector3& inter);
	Color reflection(const Vector3& inter);
	Color improvedReflection(const Vector3& inter, const Vector3& dir, DevelopmentKit& kit);
};

