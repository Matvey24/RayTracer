#include "DirectionalLight.h"
DirectionalLight::DirectionalLight(double dx, double dy, double dz, double pow, Color col) :
	dir(dx, dy, dz), pow(pow), col(col) {}
double DirectionalLight::getDistance(Vector3& pos) {
	pos.set(dir);
	return INFINITY;
}
Color DirectionalLight::getColor() {
	return col;
}
double DirectionalLight::getPower(double distance) {
	return pow;
}