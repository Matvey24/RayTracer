#include "Rectangle.h"
Intersection intersection(Wall &ud, Wall &lr, Wall &fb) {
	OperatorModel* a[] = { &ud, & lr, & fb };
	return Intersection(3, a);
}
Rectangle::Rectangle(double x, double y, double z, double w, double h, double d, MaterialModel* mm) :
	lr(Wall(x, y, z, w, mm)),
	ud(Wall(x, y, z, h, mm)),
	fb(Wall(x, y, z, d, mm)),
	in(intersection(ud, lr, fb)),
	ComplexModel(x, y, z,
		sqrt(w * w + h * h + d * d),
		&in) {
	Matrix m;
	m.setRotZ(Vector2(M_PI / 2));
	ud.rotate(sp.center, m);
	m.setRotY(Vector2(M_PI / 2));
	fb.rotate(sp.center, m);
}