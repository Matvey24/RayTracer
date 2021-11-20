#pragma once
#include "MaterialModel.h"
class Mirror :public MaterialModel
{
public:
	double reflective(const Vector3& inter);
	Color reflection(const Vector3& inter);
};