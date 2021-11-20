#pragma once
#include "MaterialModel.h"
class ColorMaterial:public MaterialModel
{
public:
	Color main_c;
	double main_ref = 0.1;
	ColorMaterial(unsigned color);
	ColorMaterial(double reflective, unsigned color);
	double reflective(const Vector3& inter);
	Color reflection(const Vector3& inter);
};

