#include "Tetrahedron.h"
Intersection intersection(HalfVolume& d, HalfVolume& l, HalfVolume& r, HalfVolume& b) {
	OperatorModel* a[] = { &d, &l, &r, &b };
	return Intersection(4, a);
}
Tetrahedron::Tetrahedron(double x, double y, double z, double s, MaterialModel* mm):
d(x, y, z, mm), 
l(x, y, z, mm),
r(x, y, z, mm),
b(x, y, z, mm),
in(intersection(this->d, l, r, b)),
ComplexModel(x, y, z, 2 * s, &in){
	Matrix m;
	m.setRotZ(Vector2(0, -1));
	Vector3 delta(0, -s / 2, 0);
	this->d.rotate(sp.center, m);
	this->d.translate(delta);

	m.setRotZ(Vector2(M_SQRT2 * 2 / 3, 1 / (double)3));
	l.rotate(sp.center, m);
	r.rotate(sp.center, m);
	b.rotate(sp.center, m);

	m.setRotY(Vector2(M_PI / 3));
	r.rotate(sp.center, m);
	m.reverse();
	l.rotate(sp.center, m);
	m.setRotY(Vector2(-1, 0));
	b.rotate(sp.center, m);

	delta.set(0, 0, s / 3 * sqrt(3 / (double)2));
	l.translate(delta);
	delta.set(0, 0, -delta.z);
	r.translate(delta);
	delta.set(s / 2 * M_SQRT1_2, 0, 0);
	b.translate(delta);
}