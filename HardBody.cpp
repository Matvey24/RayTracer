#include "HardBody.h"
HardBody::HardBody(double mass, Vector3 inert, OperatorModel* o):
	mass(mass),
	inert(inert),
	w(),
	v(),
	o(o){}
void HardBody::applyMoment(Vector3& m, double dt) {
	if (m.isZero())
		return;
	Vector3 u(m);
	u.norm();
	o->getRot().transform(u);
	Vector3 t(inert * mass);
	t.mult(u);
	w += m * (dt / t.scl(u));
}
void HardBody::update(double dt) {
	Vector3 wn(w);
	o->getRot().transform(wn);
	Vector3 eps(
		(inert.y - inert.z) / inert.x * wn.y * wn.z,
		(inert.z - inert.x) / inert.y * wn.x * wn.z,
		(inert.x - inert.y) / inert.z * wn.x * wn.y);
	wn += eps * dt;
	o->getRot().transformBack(wn);
	w = wn;
	Matrix m;
	wn *= dt;
	m.setRotOf(wn);
	o->rotate(o->getPos(), m);
}