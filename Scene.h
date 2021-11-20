#pragma once
#include "ObjectModel.h"
#include "DevelopmentKit.h"
#include "LightModel.h"
class Scene
{
private:
	LightModel** lights;
	size_t lights_len;
	size_t lights_cap;
	OperatorModel** models;
	size_t mod_len;
	size_t mod_cap;
	Color spaceColor;
	double env_light_pow = 0.1;
	const double diff = 0.000001;
public:
	Scene();
	~Scene();
	Color getColorOf(Vector3& pos, Vector3& dir, size_t depth, double scale, DevelopmentKit &kit) const;
	void addModel(OperatorModel& m);
	void addLight(LightModel& m);
private:
	SurfacePoint intersect(const Vector3& pos, const Vector3& dir, DevelopmentKit &kit) const;
	bool intersects(const Vector3& pos, const Vector3& dir, double max_len, DevelopmentKit &kit) const;
};
