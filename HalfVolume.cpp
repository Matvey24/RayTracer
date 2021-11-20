#include "HalfVolume.h"
HalfVolume::HalfVolume(double x, double y, double z,  MaterialModel* mm) :
	ObjectModel(x, y, z, mm) {}
void HalfVolume::getNorm(Vector3& internal) {
	internal.set(-1, 0, 0);
}
Segments* HalfVolume::intersectAll(const Vector3& pos, const Vector3& dir, DevelopmentKit& kit) {
	Vector3 p(center - pos);
	Vector3 d(dir);
	rot.transform(p);
	rot.transform(d);
	Segments* s = kit.getSegments();
	if (d.x == 0) {
		if (p.x < 0) {
			Segment_p seg(-INFINITY, INFINITY, this);
			s->add(seg);
		}
		return s;
	}
	double t1 = p.x / d.x;
	double t2 = INFINITY * d.x;
	Segment_p seg(fmin(t1, t2), fmax(t1, t2), this);
	s->add(seg);
	return s;
}
SPoint HalfVolume::getDistance(const Vector3& pos, const Vector3& dir, DevelopmentKit& kit) {
	Vector3 p(center - pos);
	Vector3 d(dir);
	rot.transform(p);
	rot.transform(d);
	if (d.x == 0) {
		if (p.x < 0)
			return SPoint(this, INFINITY);
		return SPoint(this, NAN);
	}
	double t1 = p.x / d.x;
	double t2 = INFINITY * d.x;
	return SPoint(this, getDist(t1, t2));
}