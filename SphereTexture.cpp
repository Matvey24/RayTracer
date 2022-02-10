#include "SphereTexture.h"
SphereTexture::SphereTexture(ImageBMP* image):im(image) {}
double SphereTexture::reflective(const Vector3& inter) {
	return main_ref;
}
Color SphereTexture::reflection(const Vector3& inter) {
	double x = -atan2(inter.x, inter.z) + M_PI;
	double y = atan2(hypot(inter.x, inter.z), inter.y);
	x *= im->width / (2 * M_PI);
	y *= im->height / M_PI;
	if (x >= im->width || x < 0 || y >= im->height || y < 0)
		return Color(0xffffff);
	return Color(im->get((int)x, (int)y));
}
