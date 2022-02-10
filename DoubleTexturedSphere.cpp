#include "DoubleTexturedSphere.h"
DoubleTexturedSphere::DoubleTexturedSphere(double x, double y, double z, double r, MaterialModel* day, MaterialModel* night, Scene* scene):
center(x, y, z), day(day), night(night), scene(scene){
	rot.setPrimary();
	rad2 = r * r;
}
Segments* DoubleTexturedSphere::intersectAll(const Vector3& pos, const Vector3& dir, DevelopmentKit& kit) {
    double k1 = dir.len2();
    Vector3 tmp(pos);
    tmp -= center;
    double k2 = -tmp.scl(dir);
    double k3 = tmp.len2() - rad2;
    double disk = k2 * k2 - k1 * k3;
    Segments* s = kit.getSegments();
    if (disk < 0)
        return s;
    disk = sqrt(disk);
    double t1 = (k2 + disk) / k1;
    double t2 = (k2 - disk) / k1;
    Segment_p v(fmin(t1, t2), fmax(t1, t2), this);
    s->add(v);
    return s;
}
SPoint DoubleTexturedSphere::getDistance(const Vector3& pos, const Vector3& dir, DevelopmentKit& kit) {
    double k1 = dir.len2();
    Vector3 tmp(pos - center);
    double k2 = -tmp.scl(dir);
    double k3 = tmp.len2() - rad2;
    double disk = k2 * k2 - k1 * k3;
    if (disk < 0)
        return SPoint(this, NAN);
    if (disk == 0) {
        double t = k2 / k1;
        if (t < 0)
            return SPoint(this, NAN);
        return SPoint(this, t);
    }
    else {
        disk = sqrt(disk);
        return SPoint(this, getDist(
            (k2 + disk) / k1,
            (k2 - disk) / k1));
    }
}
double DoubleTexturedSphere::getDist(double t1, double t2) {
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
void DoubleTexturedSphere::translate(const Vector3& vec) {
    center += vec;
}
void DoubleTexturedSphere::rotate(const Vector3& at, const Matrix& m) {
    rot = rot * m;
    Vector3 tmp(center - at);
    m.transformBack(tmp);
    center = tmp + at;
}
SurfacePoint DoubleTexturedSphere::getPoint(const Vector3& pos, const Vector3& dir, const double dist, DevelopmentKit& kit) {
    SurfacePoint saveTo;
    saveTo.intersects = dist == dist;
    if (!saveTo.intersects)
        return saveTo;
    saveTo.position = pos + dir * dist;

    Vector3 tmp(saveTo.position);
    tmp -= center;
    saveTo.norm.set(tmp);
    rot.transform(tmp);
    double scl = 1;

    double lighting = 0;
    for (size_t i = 0; i < scene->lights_len; ++i) {
        LightModel* lm = scene->lights[i];
        Vector3 dis(saveTo.position);
        Vector3 p(saveTo.position);
        double l = lm->getDistance(dis);
        p += dis * scene->diff;
        scl = saveTo.norm.scl(dis);
        if (!scene->intersects(p, dis, l, kit))
            lighting += lm->getPower(l) * scl;
    }

    if (lighting == 0) {
        saveTo.self_lighting = 0.95;
        saveTo.reflective = 0.01;
        saveTo.reflection = night->reflection(tmp);
        return saveTo;
    }

    saveTo.refraction = day->refraction;
    saveTo.transparency = day->transparency;
    Color dr = day->reflection(tmp);
    saveTo.reflection = (day->reflection(tmp));
    if (lighting < 0.3) {
        Color nr = night->reflection(tmp) / lighting;
        double scale = (0.3 - lighting) / 0.3;
        saveTo.reflection = (dr * (1 - scale) + nr * scale);
    }
    else {
        saveTo.reflection = dr;
    }
    saveTo.reflective = day->reflective(tmp);
    return saveTo;
}