#pragma once
#include "MaterialModel.h"
class ColorMaterial:public MaterialModel
{
public:
	Color main_c;
	double main_ref = 0.02;
	ColorMaterial(unsigned color);
	ColorMaterial(Color &c);
	ColorMaterial(double reflective, unsigned color);
	double reflective(const Vector3& inter);
	Color reflection(const Vector3& inter);
	Color improvedReflection(const Vector3& inter, const Vector3& dir, DevelopmentKit& kit) { return reflection(inter); }
};

