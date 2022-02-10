#include "MaterialObject.h"
MaterialObject::MaterialObject(double cx, double cy, double cz, MaterialModel* mm) :
	center(cx, cy, cz), mm(mm) {
	rot.setPrimary();
}
SurfacePoint MaterialObject::getPoint(const Vector3& pos, const Vector3& dir, double dist, DevelopmentKit& kit) {
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
    saveTo.reflective = mm->reflective(tmp);
    if (mm->improved) {
        Vector3 dir2(dir);
        rot.transform(dir2);
        saveTo.reflection = mm->improvedReflection(tmp, dir2, kit);
    }else
        saveTo.reflection = mm->reflection(tmp);

    saveTo.norm.set(tmp);
    getNorm(saveTo.norm);
    rot.transformBack(saveTo.norm);
    return saveTo;
}
void MaterialObject::translate(const Vector3& vec) {
    center += vec;
}
void MaterialObject::rotate(const Vector3& at, const Matrix& m) {
    rot = rot * m;
    Vector3 tmp(center - at);
    m.transformBack(tmp);
    center = tmp + at;
}
Matrix MaterialObject::getRot() {
    return rot;
}
Vector3 MaterialObject::getPos() {
    return center;
}
double MaterialObject::getDist(double t1, double t2) {
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