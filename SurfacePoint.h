#pragma once
#ifndef SURFACE_POINT_
#define SURFACE_POINT_
#include "Matrix.h"
#include "Color.h"
class SurfacePoint {
public:
	bool intersects = false;
	bool self_lighting = false;
	bool trans_for_diffuse = false;
	Vector3 position;
	Vector3 norm;
	double refraction = 0;
	Color diffuse;
};
#endif