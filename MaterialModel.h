#pragma once
#include "Color.h"
#include "Matrix.h"
#include "DevelopmentKit.h"
class MaterialModel {
public:
	bool improved = false;
	bool self_lighting = false;
	bool transp_for_diffuse = false;
	virtual double refraction(const Vector3& inter) = 0;
	virtual Color diffuse(const Vector3& inter) = 0;
	virtual Color improvedDiffuse(const Vector3& inter, const Vector3& dir, DevelopmentKit& kit) = 0;
};