#pragma once
#include "ConstructableGeometry.h"
class Intersection:public ConstructableGeometry
{
private:
	OperatorModel* models[10];
	size_t models_count;
public:
	Intersection(size_t count, OperatorModel* a[]);
	Segments* intersectAll(const Vector3& pos, const Vector3& dir, DevelopmentKit& kit);
	void translate(const Vector3& vec);
	void rotate(const Vector3& at, const Matrix& m);
};

