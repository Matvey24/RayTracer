#pragma once
#include "MaterialModel.h"
class Material :public MaterialModel
{
public:
	Color color;
	double refr;
	Material(double refraction, const Color& color);
	double refraction(const Vector3& inter);
	Color diffuse(const Vector3& inter);
	Color improvedDiffuse(const Vector3& inter, const Vector3& dir, DevelopmentKit& kit);
};

class Materials {
public:
	static Material defColor(const Color& color);
	static Material mirror();
	static Material light(const Color& color, double pow);
};
