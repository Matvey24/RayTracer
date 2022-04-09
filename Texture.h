#pragma once
#include "MaterialModel.h"
#include "ImageBMP.h"
class Texture: public MaterialModel
{
public:
	const ImageBMP* im;
	double refr;
	Vector3 texture_param;
	Color (&getDiffuse)(const Texture& t, const Vector3& inter);

	Texture(const ImageBMP* image, double refraction, const Vector3& texture_param,
		Color(&getDiffuse)(const Texture& t, const Vector3& inter));
	double refraction(const Vector3& inter);
	Color diffuse(const Vector3& inter);
	Color improvedDiffuse(const Vector3& inter, const Vector3& dir, DevelopmentKit& kit);
};

class Textures {
public:
	static Texture makeSpherical(const ImageBMP* im, double refraction);
	static Texture makeSpherical(const ImageBMP* im);
	
	static Texture makeRectangle(const ImageBMP* im, double refraction, const Vector3& size);
	static Texture makeRectangle(const ImageBMP* im, double w, double h, double d);

	static Texture makeThoroid(const ImageBMP* im, double refraction, double R);
	static Texture makeThoroid(const ImageBMP* im, double R);
	
	static Color rectangleAlgo(const Texture& t, const Vector3& inter);
	static Color sphericalAlgo(const Texture& t, const Vector3& inter);
	static Color thoroidAlgo(const Texture& t, const Vector3& inter);
};