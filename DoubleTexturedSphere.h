#pragma once
#include "ObjectModel.h"
#include "MaterialModel.h"
#include "Scene.h"
class DoubleTexturedSphere: public ObjectModel
{
public:
	Vector3 center;
	Matrix rot;
	MaterialModel* day, * night;
	Scene* scene;
	double rad2;
	DoubleTexturedSphere(double x, double y, double z, double r, MaterialModel* day, MaterialModel* night, Scene* scene);
	Segments* intersectAll(const Vector3& pos, const Vector3& dir, DevelopmentKit& kit);
	SPoint getDistance(const Vector3& pos, const Vector3& dir, DevelopmentKit& kit);
	void translate(const Vector3& vec);
	void rotate(const Vector3& at, const Matrix& m);
	SurfacePoint getPoint(const Vector3& pos, const Vector3& dir, const double dist, DevelopmentKit& kit);
	double getDist(double t1, double t2);
};

