#pragma once
#include "LightModel.h"
class DirectionalLight:public LightModel
{
private:
	Vector3 dir;
	double pow;
	Color col;
public:
	DirectionalLight(double dx, double dy, double dz, double pow, Color col);
	double getDistance(Vector3& pos);
	Color getColor();
	double getPower(double distance);
};

