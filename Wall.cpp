#include "Wall.h"
Wall::Wall(double x, double y, double z, double hwidth, MaterialModel* mm):
	MaterialObject(x, y, z, mm), hwidth(hwidth) {}
void Wall::getNorm(Vector3& internal) {
	internal.y = 0;
	internal.z = 0;
}
Segments* Wall::intersectAll(const Vector3& pos, const Vector3& dir, DevelopmentKit& kit) {
	Vector3 p(center - pos);
	Vector3 d(dir);
	rot.transform(p);
	rot.transform(d);
	Segments* s = kit.getSegments();
	if (d.x == 0) {
		if (p.x < hwidth && p.x > -hwidth) {
			Segment_p seg(-INFINITY, INFINITY, this);
			s->add(seg);
		}
		return s;
	}
	double t1 = (p.x + hwidth) / d.x;
	double t2 = (p.x - hwidth) / d.x;
	Segment_p seg(fmin(t1, t2), fmax(t1, t2), this);
	s->add(seg);
	return s;
}
SPoint Wall::getDistance(const Vector3& pos, const Vector3& dir, DevelopmentKit& kit) {
	Vector3 p(center - pos);
	Vector3 d(dir);
	rot.transform(p);
	rot.transform(d);
	if (d.x == 0) {
		if (p.x < hwidth && p.x > -hwidth) {
			return SPoint(this, INFINITY);
		}
		return SPoint(this, NAN);
	}
	double t1 = (p.x + hwidth) / d.x;
	double t2 = (p.x - hwidth) / d.x;
	return SPoint(this, getDist(t1, t2));
}