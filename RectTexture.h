#pragma once
#include"MaterialModel.h"
#include"ImageBMP.h"
class RectTexture:public MaterialModel
{
public:
	ImageBMP* im;
	double main_ref = 0.05;
	RectTexture(ImageBMP* image);
	double reflective(const Vector3 & inter);
	Color reflection(const Vector3 & inter);
	Color improvedReflection(const Vector3& inter, const Vector3& dir, DevelopmentKit& kit) { return reflection(inter); }
};

