#include "Pyramid.h"

Intersection intersection(HalfVolume& d, HalfVolume& l, HalfVolume& r, HalfVolume& f, HalfVolume& b) {
	OperatorModel* a[] = { &d, &f, &b, &l, &r };
	return Intersection(5, a);
}
Pyramid::Pyramid(double x, double y, double z, double w, double h, double d, MaterialModel* mm) :
	d(HalfVolume(x, y, z, mm)),
	l(HalfVolume(x, y, z, mm)),
	r(HalfVolume(x, y, z, mm)),
	f(HalfVolume(x, y, z, mm)),
	b(HalfVolume(x, y, z, mm)),
	in(intersection(this->d, l, r, f, b)),
	ComplexModel(x, y, z,
		sqrt(w * w + h * h + d * d),
		&in) {
	Matrix m;
	m.setRotZ(Vector2(0, -1));
	this->d.rotate(sp.center, m);
	Vector3 delta(0, -h, 0);
	this->d.translate(delta);

	double rad2 = 1 / (d * d + 4 * h * h);

	Vector2 v(2 * h, w);
	v *= rad2;
	m.setRotZ(v);
	f.rotate(sp.center, m);
	v.set(2 * h, -w);
	v *= rad2;
	v.rotate(Vector2(-1, 0));
	m.setRotZ(v);
	b.rotate(sp.center, m);

	delta.set(-w / 2, 0, 0);
	f.translate(delta);
	delta.set(w / 2, 0, 0);
	b.translate(delta);

	m.setRotY(Vector2(0, 1));
	r.rotate(sp.center, m);
	m.setRotY(Vector2(0, -1));
	l.rotate(sp.center, m);
	v.set(2 * h, d);
	v *= rad2;
	m.setRotX(v);
	l.rotate(sp.center, m);
	m.reverse();
	r.rotate(sp.center, m);

	delta.set(0, 0, d / 2);
	l.translate(delta);
	delta.set(0, 0, -d / 2);
	r.translate(delta);
}