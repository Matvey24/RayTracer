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
    double reflective = pow(surf.reflective, -scl);
    double diffuse = 1 - reflective;
    if (reflective * kit.scale < diffuse_imp) {
        diffuse = 1;
        reflective = 0;
    }
    double reflect_scale = kit.scale * reflective;
    double diffuse_scale = kit.scale * (1 - reflective);
    if (surf.self_lighting)
        return surf.reflection;
    else if (kit.depth != 0) {
#ifdef FULL_REFLECTIONS
        int disc = diffuse_scale / diffuse_imp;
        if (disc == 0)
            disc = 1;
        int height = (int)sqrt(disc);
        if(height == 0)
            height = 1;
        int width = disc / height;
        disc = height * width;


        double cv = diffuse / disc;

        double scale = kit.scale;
        kit.scale = diffuse_scale / disc;
        kit.depth -= 1;
        Matrix m;
        Vector3 dir2(surf.norm);
        dir2 = dir2 * Vector3(1, 0, 0);
        if (dir2.isZero())
            dir2.set(0, 1, 0);
        else
            dir2.norm();
        m.setRotE(dir2, Vector2(surf.norm.x, sqrt(1 - surf.norm.x * surf.norm.x)));
        for (int h = 0; h < height; ++h) {
            for (int w = 0; w < width; ++w) {
                double cosphi = (rand() / 32768. * (1 - min_surf_ang) + min_surf_ang + h) / height;
                double sinphi = sqrt(1 - cosphi * cosphi);
                double psi = (rand() / 32768. * (1 - min_surf_ang) + min_surf_ang + w) * 2 * M_PI / width;
                Vector3 dir2(cosphi, sinphi * sin(psi), sinphi * cos(psi));
                m.transformBack(dir2);
                Vector3 pos2(surf.position + dir2 * diff);
                result += surf.reflection.reflect(getColorOf(pos2, dir2, kit)) * cv;
            }
        }
        kit.scale = reflect_scale;
        if (kit.scale >= diffuse_imp) {
            dir2.set(dir - surf.norm * (2 * scl));
            Vector3 pos2(surf.position + dir2 * diff);
            result += getColorOf(pos2, dir2, kit) * reflective;
        }
        kit.depth += 1;
        kit.scale = scale;
#else
        //reflections
        if (reflective != 0) {
            double scale = kit.scale;
            kit.scale = reflect_scale;
            kit.depth -= 1;
            Vector3 dir2(dir - surf.norm * (2 * scl));
            Vector3 pos2(surf.position + dir2 * diff);
            result += getColorOf(pos2, dir2, kit) * reflective;
            kit.depth += 1;
            kit.scale = scale;
        }
        //lights
        if (reflective != 1) {
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
                    result += surf.reflection.reflect(c) * (power * diffuse_scale);
                }
            }
        }
        result += surf.reflection * (env_light_pow * diffuse_scale);
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
