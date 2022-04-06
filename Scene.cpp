#include "Scene.h"
//#define FULL_REFLECTIONS
Scene::Scene(MaterialModel* space):space(space) {
	mod_cap = 10;
	models = (OperatorModel**)malloc(sizeof(OperatorModel*) * mod_cap);
	mod_len = 0;
    lights_cap = 10;
    lights = (LightModel**)malloc(sizeof(LightModel*) * lights_cap);
    lights_len = 0;
}
Scene::~Scene() {
    free(models);
    free(lights);
}
Color Scene::simpleLight(Vector3& pos2, Vector3& dir2, DevelopmentKit& kit) const {
    Color color(1, 1, 1);
    Vector3 pos(pos2);
    Vector3 dir(dir2);
    for (int i = 0; i < kit.depth; ++i) {
        struct SurfacePoint surf = intersect(pos, dir, kit);
        surf.norm.norm();
        if (!surf.intersects)
            return color.reflect(space->reflection(dir));
        else if (surf.self_lighting)
            return color.reflect(surf.reflection);
        double scl = dir.scl(surf.norm);
        double refl = calcReflect(scl, surf.reflective);
        if (rand() / 32767. > refl) {//diffuse
            double cosphi = rand() / 32767.;
            double sinphi = sqrt(1 - cosphi * cosphi);
            double psi = rand() / 32767. * 2 * M_PI;
            dir.set(cosphi, sinphi * sin(psi), sinphi * cos(psi));
            Vector3 dir2(surf.norm * Vector3(1, 0, 0));
            if (dir2.isZero())
                dir2.set(0, 1, 0);
            else
                dir2.norm();
            struct Matrix m;
            m.setRotE(dir2, Vector2(surf.norm.x, sqrt(1 - surf.norm.x * surf.norm.x)));
            m.transformBack(dir);
            color = color.reflect(surf.reflection);
        }
        else {//reflect
            dir = dir - surf.norm * (2 * scl);
        }
        pos = surf.position + dir * diff;
    }
    return Color(0, 0, 0);
}
Color Scene::updateDiffuse(Vector3& pos, Vector3& norm, DevelopmentKit& kit, int disc) const {
    Vector3 dir2(norm * Vector3(1, 0, 0));
    if (dir2.isZero())
        dir2.set(0, 1, 0);
    else
        dir2.norm();
    struct Matrix m;
    m.setRotE(dir2, Vector2(norm.x, sqrt(1 - norm.x * norm.x)));
    if (disc == 0)
        disc = 1;
    int width = sqrt((float)disc);
    int height = disc / width;
    double scale = 1. / (width * height);
    Color color;

    for (int x = 0; x < width; ++x) {
        for (int y = 0; y < height; ++y) {
            double cosphi = (rand() / 32767. + y) / height;
            double sinphi = sqrt(1 - cosphi * cosphi);
            double psi = (rand() / 32767. + x) * 2 * M_PI / width;
            Vector3 dir(cosphi, sinphi * sin(psi), sinphi * cos(psi));
            m.transformBack(dir);
            Vector3 pos2 = pos + dir * diff;
            color += simpleLight(pos2, dir, kit) * scale;
        }
    }
    return color;
}
Color Scene::getColorOf(Vector3& pos, Vector3& dir, DevelopmentKit &kit) const {
	dir.norm();
    SurfacePoint surf = intersect(pos, dir, kit);
    if(!surf.intersects)
	    return space->reflection(dir);
    if (surf.self_lighting)
        return surf.reflection;
    surf.norm.norm();
    Color result;
    double scl = dir.scl(surf.norm);
    if (scl > 0) {
        surf.norm = -surf.norm;
        scl = -scl;
    }
    double refl = calcReflect(scl, surf.reflective);
    
    if (kit.depth != 0) {
#ifdef FULL_REFLECTIONS
        if (refl != 1) {
            int discDiffuse = (1 - refl) * kit.scale / diffuse_imp;
            Color diffuse = updateDiffuse(surf.position,
                surf.norm, kit, discDiffuse);
            result = surf.reflection.reflect(diffuse) * (1 - refl);
        }
        kit.scale *= refl;
        if (kit.scale < diffuse_imp)
            return result;
        kit.depth--;
        Vector3 dir2(dir - surf.norm * (2 * scl));
        Vector3 pos2(surf.position + dir2 * diff);
        result += getColorOf(pos2, dir2, kit) * refl;
        kit.depth++;
#else
        //reflections
        if (refl != 0 && kit.scale > diffuse_imp) {
            double scale = kit.scale;
            kit.scale = refl * scale;
            kit.depth -= 1;
            Vector3 dir2(dir - surf.norm * (2 * scl));
            Vector3 pos2(surf.position + dir2 * diff);
            result += getColorOf(pos2, dir2, kit) * refl;
            kit.depth += 1;
            kit.scale = scale;
        }
        //lights
        if (refl != 1) {
            for (size_t i = 0; i < lights_len; ++i) {
                Vector3 d(surf.position);
                Vector3 p(d);
                double distance = lights[i]->getDistance(d);
                d.norm();
                p += d * diff;
                if (!intersects(p, d, distance, kit)) {
                    Color c = lights[i]->getColor();
                    double power = lights[i]->getPower(distance);
                    power *= surf.norm.scl(d);
                    if (power < 0)
                        power = -power;
                    result += surf.reflection.reflect(c) 
                        * (power * (1 - refl));
                }
            }
        }
        result += surf.reflection * (env_light_pow * (1 - refl));
#endif
    }
    return result;
}
void Scene::addModel(OperatorModel& m) {
	if (mod_len == mod_cap) {
		OperatorModel **m = (OperatorModel**)realloc(models, mod_cap * 2);
        if (m == NULL) {
            exit(1);
            return;
        }else
            mod_cap *= 2;
        models = m;
	}
	models[mod_len++] = &m;
}
void Scene::addLight(LightModel& m) {
    if (lights_len == lights_cap) {
        LightModel** m = (LightModel**)realloc(lights, sizeof(LightModel*) * lights_cap * 2);
        if (m == NULL) {
            exit(1);
            return;
        }
        else
            lights_cap *= 2;
        lights = m;
    }
    lights[lights_len++] = &m;
}
SurfacePoint Scene::intersect(const Vector3& pos, const Vector3& dir, DevelopmentKit &kit) const {
    OperatorModel* om;
    SPoint sp = getNearestModel(models, mod_len, pos, dir, kit, &om);
    if (sp.model != NULL)
        return sp.model->getPoint(pos, dir, sp.pos, kit);
    return SurfacePoint();
}
bool Scene::intersects(const Vector3& pos, const Vector3& dir, double max_len, DevelopmentKit &kit) const {
    for (size_t i = 0; i < mod_len; ++i) {
        OperatorModel* om = models[i];
        SPoint dist = om->getDistance(pos, dir, kit);
        if (dist.pos <= max_len)
            return true;
    }
    return false;
}
double Scene::calcReflect(double sclDirNorm, double ref) const{
    ref = 1 - ref;
    double cosfi = (sclDirNorm > 0? sclDirNorm:-sclDirNorm);
    double sinfi = sqrt(1 - cosfi * cosfi);
    double sinpsi = ref * sinfi;
    double cospsi = sqrt(1 - sinpsi * sinpsi);
    double rpe = (cosfi - ref * cospsi) / ((cosfi + ref * cospsi));
    double rpa = (ref * cosfi - cospsi) / (ref * cosfi + cospsi);
    return sqrt((rpe * rpe + rpa * rpa) / 2);
}