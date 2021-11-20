#include "ObjectModel.h"
ObjectModel::ObjectModel(double cx, double cy, double cz, MaterialModel* mm) :
	center(cx, cy, cz), mm(mm) {
	type = 1;
	rot.setPrimary();
}
SurfacePoint ObjectModel::getPoint(const Vector3& pos, const Vector3& dir, const double dist) {
    SurfacePoint saveTo;
    saveTo.intersects = dist == dist;
    if (!saveTo.intersects)
        return saveTo;

    Vector3 tmp(pos);
    tmp += dir * dist;
    saveTo.position = tmp;
    tmp -= center;
    rot.transform(tmp);

    saveTo.refraction = mm->refraction;
    saveTo.transparency = mm->transparency;
    saveTo.reflection = mm->reflection(tmp);
    saveTo.reflective = mm->reflective(tmp);

    saveTo.norm.set(tmp);
    getNorm(saveTo.norm);
    rot.transformBack(saveTo.norm);
    return saveTo;
}
SurfacePoint ObjectModel::intersect(const Vector3& pos, const Vector3& dir, DevelopmentKit& kit) {
    return getPoint(pos, dir, getDistance(pos, dir, kit).pos);
}
void ObjectModel::translate(const Vector3& vec) {
    center += vec;
}
void ObjectModel::rotate(const Vector3& at, const Matrix& m) {
    rot = rot * m;
    Vector3 tmp(center - at);
    m.transform(tmp);
    center = tmp + at;
}
double ObjectModel::getDist(double t1, double t2) {
    if (t1 < 0 && t2 < 0)
        return NAN;
    if (t1 > t2) {
        double t = t1;
        t1 = t2;
        t2 = t;
    }
    if (t1 < 0)
        return t2;
    return t1;
}
SPoint getNearestModel(OperatorModel** models, size_t count, const Vector3& pos, const Vector3& dir, DevelopmentKit& kit, OperatorModel** oper) {
    SPoint sp(NULL, INFINITY);
    OperatorModel* op = NULL;
    for (size_t i = 0; i < count; ++i) {
        SPoint new_dist = models[i]->getDistance(pos, dir, kit);
        if (new_dist.pos != new_dist.pos || new_dist.pos <= 0)
            continue;
        if (new_dist.pos <= sp.pos) {
            sp = new_dist;
            op = models[i];
        }
    }
    if (sp.pos == INFINITY)
        sp.pos = NAN;
    *oper = op;
    return sp;
}