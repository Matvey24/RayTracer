#include "ConstructableGeometry.h"
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