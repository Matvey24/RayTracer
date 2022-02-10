#pragma once
#include "Matrix.h"
#include "MaterialObject.h"
class HardBody
{
public:
	double mass;
	const Vector3 inert;//internal
	Vector3 w;//absolute
	Vector3 v;//absolute
	OperatorModel* o;

	HardBody(double mass, Vector3 inert, OperatorModel* o);
	void update(double dt);
	void applyMoment(Vector3& m, double dt);
};

