#include "Material.h"
Material::Material(double refraction, const Color& color) :
	refr(refraction), color(color) {}
double Material::refraction(const Vector3& inter) {
	return refr;
}
Color Material::diffuse(const Vector3& inter) {
	return color;
}
Color Material::improvedDiffuse(const Vector3& inter, const Vector3& dir, DevelopmentKit& kit) {
	return color;
}

Material Materials::defColor(const Color& color) {
	return Material(0.1, color);
}
Material Materials::mirror() {
	return Material(1, Color(1, 1, 1));
}
Material Materials::light(const Color& color, double pow) {
	Material m(1, color * pow);
	m.self_lighting = true;
	return m;
}
