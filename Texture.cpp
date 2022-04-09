#include "Texture.h"
Texture::Texture(const ImageBMP* image, double refraction, const Vector3& texture_param,
	Color(&getDiffuse)(const Texture& t, const Vector3& inter)):
	im(image), refr(refraction), texture_param(texture_param), getDiffuse(getDiffuse){}
double Texture::refraction(const Vector3& inter) {
	return refr;
}
Color Texture::diffuse(const Vector3& inter) {
	return getDiffuse(*this, inter);
}
Color Texture::improvedDiffuse(const Vector3& inter, const Vector3& dir, DevelopmentKit& kit) {
	return getDiffuse(*this, inter);
}

Texture Textures::makeSpherical(const ImageBMP* im, double refraction) {
	return Texture(im, refraction, Vector3(), sphericalAlgo);
}
Texture Textures::makeRectangle(const ImageBMP* im, double refraction, const Vector3& size) {
	return Texture(im, refraction, size, rectangleAlgo);
}
Texture Textures::makeThoroid(const ImageBMP* im, double refraction, double R) {
	return Texture(im, refraction, Vector3(R, 0, 0), thoroidAlgo);
}

Texture Textures::makeSpherical(const ImageBMP* im) {
	return makeSpherical(im, 0.1);
}
Texture Textures::makeRectangle(const ImageBMP* im, double w, double h, double d) {
	return makeRectangle(im, 0.1, Vector3(w, h, d));
}
Texture Textures::makeThoroid(const ImageBMP* im, double R) {
	return makeThoroid(im, 0.1, R);
}


Color Textures::rectangleAlgo(const Texture& t, const Vector3& in) {
	const ImageBMP* im = t.im;
	Vector3 inter(in.x / t.texture_param.x, in.y / t.texture_param.y, in.z / t.texture_param.z);
	double x = im->width / 2 - inter.z / 2 * (im->width - 1);
	double y = im->height / 2 - inter.y / 2 * (im->height - 1);
	if (x < 0 || x >= im->width - 1 || y < 0 || y >= im->height - 1)
		return Color();
	Color a1 = im->get(floor(x), floor(y));
	Color a2 = im->get(floor(x), ceil(y));
	Color b1 = im->get(ceil(x), floor(y));
	Color b2 = im->get(ceil(x), ceil(y));
	double a = y - floor(y);
	double b = x - floor(x);
	return (a1 * (1 - a) + a2 * a) * (1 - b) + (b1 * (1 - a) + b2 * a) * b;
}
Color Textures::sphericalAlgo(const Texture& t, const Vector3& inter) {
	double x = -atan2(inter.x, inter.z) + M_PI;
	double y = atan2(hypot(inter.x, inter.z), inter.y);
	x *= t.im->width / (2 * M_PI);
	y *= t.im->height / M_PI;
	if (x >= t.im->width || x < 0 || y >= t.im->height || y < 0)
		return Color(0xffffff);
	return Color(t.im->get((int)x, (int)y));
}
Color Textures::thoroidAlgo(const Texture& t, const Vector3& inter) {
	Vector3 tm(inter);
	tm.y = 0;
	tm *= t.texture_param.x / tm.len();
	double h = hypot(inter.x, inter.z);
	double x = ((h >t.texture_param.x) ? -1 : 1) * atan2(inter.x, inter.z) + M_PI;
	x *= t.im->width / (2 * M_PI);
	Vector3 ty(inter);
	ty -= tm;
	double y = atan2(hypot(ty.x, ty.z), ty.y);
	y *= t.im->height / M_PI;
	if (x >= t.im->width || x < 0 || y >= t.im->height || y < 0)
		return Color(0xffffff);
	return Color(t.im->get((int)x, (int)y));
}