#include "RectTexture.h"
RectTexture::RectTexture(ImageBMP* image) :im(image){}
double RectTexture::reflective(const Vector3& inter) {
	return main_ref;
}
Color RectTexture::reflection(const Vector3& inter) {
	unsigned x = (unsigned)(im->width / 2 - inter.z / 2 * (im->width - 1));
	unsigned y = (unsigned)(im->height / 2 - inter.y / 2 * (im->height - 1));
	if (x < 0 || x >= im->width || y < 0 || y >= im->height) {
		return Color();
	}

	return Color(im->get(x, y));
}