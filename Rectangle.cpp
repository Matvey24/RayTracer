#include "Rectangle.h"
Rectangle::Rectangle(double x, double y, double z, double w, double h, double d, MaterialModel* mm):
MaterialObject(x, y, z, mm), w(w), h(h), d(d){}
void Rectangle::getNorm(Vector3& i){
	double x = i.x / w, y = i.y / h, z = i.z / d;
	x *= x;
	y *= y;
	z *= z;
	bool xz = x > z, xy = x > y, yz = y > z;
	i.set(
		xz && xy,
		!xy && yz,
		!xz && !yz);
}
inline double abs(double a) {
	return (a > 0) ? a : -a;
}
Segments* Rectangle::intersectAll(const Vector3& pos, const Vector3& dir, DevelopmentKit& kit) {
	Vector3 p(center - pos);
	Vector3 d(dir);
	rot.transform(p);
	rot.transform(d);
	if (d.x == 0 && abs(p.x) > w || d.y == 0 && abs(p.y) > h || d.z == 0 && abs(p.z) > this->d)
		return kit.getSegments();
	double s1, s2, s3, e1, e2, e3;
	double t1 = (p.x + w) / d.x;
	double t2 = (p.x - w) / d.x;
	s1 = fmin(t1, t2);
	e1 = fmax(t1, t2);
	t1 = (p.y + h) / d.y;
	t2 = (p.y - h) / d.y;
	s2 = fmin(t1, t2);
	e2 = fmax(t1, t2);
	t1 = (p.z + this->d) / d.z;
	t2 = (p.z - this->d) / d.z;
	s3 = fmin(t1, t2);
	e3 = fmax(t1, t2);
	t1 = fmax(fmax(s1, s2), s3);
	t2 = fmin(fmin(e1, e2), e3);
	if (t1 > t2 || t2 < 0)
		return kit.getSegments();
	Segment_p seg(t1, t2, this);
	Segments* s = kit.getSegments();
	s->add(seg);
	return s;
}
SPoint Rectangle::getDistance(const Vector3& pos, const Vector3& dir, DevelopmentKit& kit) {
	Vector3 p(center - pos);
	Vector3 d(dir);
	rot.transform(p);
	rot.transform(d);
	if (d.x == 0 && abs(p.x) > w || d.y == 0 && abs(p.y) > h || d.z == 0 && abs(p.z) > this->d)
		return SPoint(this, NAN);
	double s1, s2, s3, e1, e2, e3;
	double t1 = (p.x + w) / d.x;
	double t2 = (p.x - w) / d.x;
	s1 = fmin(t1, t2);
	e1 = fmax(t1, t2);
	t1 = (p.y + h) / d.y;
	t2 = (p.y - h) / d.y;
	s2 = fmin(t1, t2);
	e2 = fmax(t1, t2);
	t1 = (p.z + this->d) / d.z;
	t2 = (p.z - this->d) / d.z;
	s3 = fmin(t1, t2);
	e3 = fmax(t1, t2);
	t1 = fmax(fmax(s1, s2), s3);
	t2 = fmin(fmin(e1, e2), e3);
	if (t1 > t2 || t2 < 0)
		return SPoint(this, NAN);
	return SPoint(this, t1);
}
