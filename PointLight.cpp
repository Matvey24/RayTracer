#include "PointLight.h"
PointLight::PointLight(double x, double y, double z, double pow, Color col) :
	pos(x, y, z), pow(pow), col(col) {}
double PointLight::getDistance(Vector3& pos) {
	pos = this->pos - pos;
	return pos.len();
}
Color PointLight::getColor() {
	return col;
}
double PointLight::getPower(double distance) {
	return pow / distance * distance;
}