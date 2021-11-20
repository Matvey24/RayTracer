#pragma once
#include "LightModel.h"
class PointLight:public LightModel
{
	double pow;
	Color col;
	Vector3 pos;
public:
	PointLight(double x, double y, double z, double pow, Color col);
	double getDistance(Vector3& pos);
	Color getColor();
	double getPower(double distance);
};

