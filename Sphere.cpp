#include "Sphere.h"
Sphere::Sphere(double x, double y, double z, double rad, MaterialModel* mm) :
	MaterialObject(x, y, z, mm), rad2(rad* rad) {}
void Sphere::getNorm(Vector3& internal) {}
SPoint Sphere::getDistance(const Vector3& pos, const Vector3& dir, DevelopmentKit& kit) {
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
Segments* Sphere::intersectAll(const Vector3& pos, const Vector3& dir, DevelopmentKit& kit) {
    double k1 = dir.len2();
    Vector3 tmp(pos);
    tmp -= center;
    double k2 = -tmp.scl(dir);
    double k3 = tmp.len2() - rad2;
    double disk = k2 * k2 - k1 * k3;
    Segments *s = kit.getSegments();
    if (disk < 0)
        return s;
    disk = sqrt(disk);
    double t1 = (k2 + disk) / k1;
    double t2 = (k2 - disk) / k1;
    Segment_p v(fmin(t1, t2), fmax(t1, t2), this);
    s->add(v);
    return s;
}