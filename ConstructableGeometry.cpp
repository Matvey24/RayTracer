#include "ConstructableGeometry.h"
SurfacePoint ConstructableGeometry::intersect(const Vector3& pos, const Vector3& dir, DevelopmentKit& kit) {
	Segments* s = intersectAll(pos, dir, kit);
	SPoint* point = s->getFirstActive();
	if (point == NULL) {
		kit.pushSegment(s);
		return SurfacePoint();
	}
	SurfacePoint sp = point->model->getPoint(pos, dir, point->pos);
	kit.pushSegment(s);
	return sp;
}
SPoint ConstructableGeometry::getDistance(const Vector3& pos, const Vector3& dir, DevelopmentKit& kit) {
	Segments* s = intersectAll(pos, dir, kit);
	SPoint* point = s->getFirstActive();
	if (point == NULL) {
		kit.pushSegment(s);
		return SPoint(NULL, NAN);
	}
	SPoint val = *point;
	kit.pushSegment(s);
	return val;
}