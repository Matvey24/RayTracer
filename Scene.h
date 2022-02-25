#pragma once
#ifndef SCENE_H_
#define SCENE_H_
#include "MaterialObject.h"
#include "DevelopmentKit.h"
#include "LightModel.h"
class Scene
{
private:
	size_t lights_cap;
	OperatorModel** models;
	size_t mod_len;
	size_t mod_cap;
	MaterialModel* space;
public:
	const double diff = 0.001;
	const double min_surf_ang = 0.001;
	const double diffuse_imp = 1 / 1000.;

	size_t lights_len;
	LightModel** lights;
	double env_light_pow = 0.2;
	Scene(MaterialModel* space);
	~Scene();
	Color getColorOf(Vector3& pos, Vector3& dir, DevelopmentKit &kit) const;
	void addModel(OperatorModel& m);
	void addLight(LightModel& m);
	bool intersects(const Vector3& pos, const Vector3& dir, double max_len, DevelopmentKit& kit) const;
private:
	SurfacePoint intersect(const Vector3& pos, const Vector3& dir, DevelopmentKit &kit) const;
};
#endif