#include "Thor.h"
Thor::Thor(double x, double y, double z, double rad, double RAD, MaterialModel* mm) :
	MaterialObject(x, y, z, mm), rad(rad), RAD(RAD) {}
void Thor::getNorm(Vector3& internal) {
	Vector3 t(internal);
	t.y = 0;
	t *= (RAD / t.len());
	internal -= t;
}
int compareDouble(const void* a, const void* b) {
	return (*(double*)a < *(double*)b) ? -1 : 1;
}
unsigned Thor::getRoots(const Vector3& pos, const Vector3& dir, double* x) const {
	double R2 = RAD * RAD;
	double a = dir.len2();
	double b = 2 * dir.scl(pos);
	double c = pos.len2() + R2 - rad * rad;
	double d = 4 * R2 * (dir.x * dir.x + dir.z * dir.z);
	double e = 8 * R2 * (dir.x * pos.x + dir.z * pos.z);
	double f = 4 * R2 * (pos.x * pos.x + pos.z * pos.z);

	double g = a * a;
	double h = 2 * a * b;
	double i = (2 * a * c + b * b - d);
	double j = (2 * b * c - e);
	double k = c * c - f;
	unsigned count = Equation().root4f(g, h, i, j, k, x);
	qsort(x, count, sizeof(double), compareDouble);
	return count;
}
inline bool Thor::simple_check(const Vector3& pos, const Vector3& dir) const{
	double k1 = dir.len2();
	Vector3 tmp(pos);
	tmp -= center;
	double k2 = -tmp.scl(dir);
	double k3 = tmp.len2() - (rad + RAD) * (rad + RAD);
	return k2 * k2 - k1 * k3 < 0;
}
SPoint Thor::getDistance(const Vector3& pos, const Vector3& dir, DevelopmentKit& kit) {
	if (simple_check(pos, dir))
		return SPoint(this, NAN);
	Vector3 p(pos - center);
	Vector3 d(dir);
	rot.transform(p);
	rot.transform(d);
	double roots[4];
	unsigned count = getRoots(p, d, roots);
	for (unsigned i = 0; i < count; ++i) {
		if (roots[i] > 0) {
			return SPoint(this, roots[i]);
		}
	}
	return SPoint(this, NAN);
}
Segments* Thor::intersectAll(const Vector3& pos, const Vector3& dir, DevelopmentKit& kit) {
	if (simple_check(pos, dir))
		return kit.getSegments();
	Vector3 p(pos - center);
	Vector3 d(dir);
	rot.transform(p);
	rot.transform(d);
	double roots[4];
	unsigned count = getRoots(p, d, roots);
	if (count == 1 || count == 3) {
		
	}
	else {
		Segments* s = kit.getSegments();
		for (unsigned i = 0; i < count; i += 2) {
			if (roots[i + 1] > 0) {
				Segment_p sep(SPoint(this, roots[i]), SPoint(this, roots[i + 1]));
				s->add(sep);
			}
		}
		return s;
	}
	return kit.getSegments();
}