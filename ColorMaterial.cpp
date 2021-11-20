#include "ColorMaterial.h"
ColorMaterial::ColorMaterial(unsigned color):
main_c(color){}
ColorMaterial::ColorMaterial(double reflective, unsigned color) :
	main_ref(reflective), main_c(color) {}
double ColorMaterial::reflective(const Vector3& inter){
	return main_ref;
}
Color ColorMaterial::reflection(const Vector3& inter) {
	return main_c;
}