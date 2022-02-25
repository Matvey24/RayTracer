#include "MandelBulb.h"
MandelBulb::MandelBulb(double x, double y, double z, double rad, MaterialModel* mm):
MaterialObject(x, y, z, mm), rad(rad), sp(x, y, z, rad, mm){}
double MandelBulb::DE(Vector3& internal) {
	Vector3 rad(internal);
	float dr = 1;
	float r = 0;
	for (size_t i = 0; i < fract_iter; ++i) {
		r = rad.len();
		if (r > Bailout)
			break;
		float theta = acos(rad.z / r);
		float phi = atan2(rad.y, rad.x);
		dr = pow(r, power - 1) * power * dr + 1;
		float zr = pow(r, power);
		theta = theta * power;
		phi = phi * power;
		rad.set(sin(theta) * cos(phi), sin(theta) * sin(phi), cos(theta));
		rad *= zr;
		rad += internal;
	}
	return 0.5 * log(r) * r / dr;
}
void MandelBulb::getNorm(Vector3& internal){
	Vector3 xp(internal + Vector3(diff, 0, 0));
	Vector3 xn(internal - Vector3(diff, 0, 0));
	Vector3 yp(internal + Vector3(0, diff, 0));
	Vector3 yn(internal - Vector3(0, diff, 0));
	Vector3 zp(internal + Vector3(0, 0, diff));
	Vector3 zn(internal - Vector3(0, 0, diff));
	internal.set(
		DE(xp) - DE(xn),
		DE(yp) - DE(yn),
		DE(zp) - DE(zn));
	internal = internal * (1 / (2 * diff));
}
SPoint MandelBulb::getDistance(const Vector3& pos, const Vector3& dir, DevelopmentKit& kit) {
	SPoint dist = sp.getDistance(pos, dir, kit);
	if (dist.pos != dist.pos)
		return SPoint(NULL, NAN);
	double d = diff;
	double max_d = dist.pos;
	if ((pos - sp.center).len2() >= sp.rad2) {
		d = dist.pos;
		max_d = 2 * rad;
	}
	Vector3 from((pos + dir * d) - sp.center);
	Vector3 direction(dir);
	rot.transform(from);
	rot.transform(direction);
	float total_distance = 0;
	size_t steps;
	for(steps = 0; steps < raymarch_steps; ++steps){
		Vector3 p(from + direction * total_distance);
		float distance = DE(p);
		total_distance += distance;
		if (distance < minimum_distance)
			return SPoint(this, total_distance + d);
		if (total_distance > max_d)
			return SPoint(NULL, NAN);
	}
	return SPoint(NULL, NAN);
}
Segments* MandelBulb::intersectAll(const Vector3& pos, const Vector3& dir, DevelopmentKit& kit) {
	return kit.getSegments();
}
