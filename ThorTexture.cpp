#include "ThorTexture.h"
ThorTexture::ThorTexture(ImageBMP* image, double R) :im(image), R(R) {}
double ThorTexture::reflective(const Vector3& inter) {
	return main_ref;
}
Color ThorTexture::reflection(const Vector3& inter) {
	Vector3 t(inter);
	t.y = 0;
	t *= (R / t.len());
	double h = hypot(inter.x, inter.z);
	double x = ((h > R)?-1:1) * atan2(inter.x, inter.z) + M_PI;
	x *= im->width / (2 * M_PI);
	Vector3 ty(inter);
	ty -= t;
	double y = atan2(hypot(ty.x, ty.z), ty.y);
	y *= im->height / M_PI;
	if (x >= im->width || x < 0 || y >= im->height || y < 0)
		return Color(0xffffff);
	return Color(im->get((int)x, (int)y));
}