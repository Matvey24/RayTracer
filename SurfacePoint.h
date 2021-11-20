#pragma once
#include "Matrix.h"
#include "MaterialModel.h"
#include <corecrt_math_defines.h>
class SurfacePoint {
public:
	bool intersects = false;
	Vector3 position;
	Vector3 norm;
	double refraction = 0;
	Color transparency;
	double reflective = 0;
	Color reflection;
};