#include "RectTexture.h"
RectTexture::RectTexture(ImageBMP* image) :im(image){}
double RectTexture::reflective(const Vector3& inter) {
	return main_ref;
}
Color RectTexture::reflection(const Vector3& in) {
	Vector3 inter = in * 0.9;
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