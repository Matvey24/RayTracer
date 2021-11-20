#pragma once
#ifndef _LIGHTS_
#define _LIGHTS_
#include "SurfacePoint.h"
class LightModel {
public:
	virtual double getDistance(Vector3& pos) = 0;
	virtual Color getColor() = 0;
	virtual double getPower(double distance) = 0;
};
#endif