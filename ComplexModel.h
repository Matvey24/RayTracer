#pragma once
#include "Sphere.h"
class ComplexModel:public OperatorModel
{
protected:
	Sphere sp;
	OperatorModel* models[10];
	size_t model_count;
public:
	ComplexModel(double x, double y, double z, double rad, size_t models_count, OperatorModel* a[]);
	ComplexModel(double x, double y, double z, double rad, OperatorModel* a);
	Segments* intersectAll(const Vector3& pos, const Vector3& dir, DevelopmentKit& kit);
	SPoint getDistance(const Vector3& pos, const Vector3& dir, DevelopmentKit& kit);
	void translate(const Vector3& vec);
	void rotate(const Vector3& at, const Matrix& m);
	Matrix getRot();
	Vector3 getPos();
};

