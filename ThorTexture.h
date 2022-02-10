#pragma once
#include"MaterialModel.h"
#include"ImageBMP.h"
class ThorTexture: public MaterialModel
{
public:
	ImageBMP* im;
	double main_ref = 0.01;
	double R;
	ThorTexture(ImageBMP* image, double R);
	double reflective(const Vector3& inter);
	Color reflection(const Vector3& inter);
	Color improvedReflection(const Vector3& inter, const Vector3& dir, DevelopmentKit& kit) { return reflection(inter); }
};

