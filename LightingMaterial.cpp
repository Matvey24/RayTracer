#include "LightingMaterial.h"
LightingMaterial::LightingMaterial(unsigned color, double pow):
main_c(color){
	self_lighting = true;
	main_c = main_c * pow;
}
LightingMaterial::LightingMaterial(Color& c, double pow) :
	main_c(c) {
	self_lighting = true;
	main_c = main_c * pow;
}
double LightingMaterial::reflective(const Vector3& inter) {
	return 1;
}
Color LightingMaterial::reflection(const Vector3& inter) {
	return main_c;
}
Color LightingMaterial::improvedReflection(const Vector3& inter, const Vector3& dir, DevelopmentKit& kit) {
	return main_c;
}