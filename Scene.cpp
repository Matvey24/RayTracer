#include "Scene.h"
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
    double reflective = pow(surf.reflective, (scl < 0) ? -scl : scl);
    if (kit.depth != 0) {
#ifdef FULL_REFLECTIONS
        int discretization = 11 * (1 - reflective);
        double scale2, delta;
        do {
            discretization--;
            delta = (double)1 / (discretization * 2 * discretization);
            scale2 = scale * delta;
        } while (discretization > 3 && scale2 < 0.01);
        if (discretization < 4) {
            Vector3 dir2(dir - surf.norm * (2 * scl));
            Vector3 pos2(surf.position + dir2 * diff);
            result += getColorOf(pos2, dir2, depth - 1, scale, kit) * surf.reflective;
        }
        else {
            Matrix mat;
            Vector3 dir2(dir - surf.norm * (2 * scl));
            for (int i = 0, m = discretization * 2; i < m; ++i) {
                Matrix vert_mat;
                    vert_mat.setRotZ(Vector2(M_PI * i / m));
                    for (int j = 0; j < discretization; ++j) {
                        mat.setRotY(Vector2(2 * M_PI * (1 - pow(j / discretization, 1 / (1 - reflective)))));
                        mat = mat * vert_mat;
                        Vector3 dir3(dir2);
                        mat.transform(dir3);
                        if (dir3.scl(surf.norm) < 0)
                            continue;
                        Vector3 pos2(surf.position + dir3 * diff);
                        result += getColorOf(pos2, dir3, depth - 1, scale2, kit) * surf.reflective * delta;

                    }
            }
        }
        
#else
        //reflections
        if (surf.reflective != 0) {
            double scale2 = kit.scale * reflective;
            if (scale2 >= 0.01) {
                Vector3 dir2(dir - surf.norm * (2 * scl));
                Vector3 pos2(surf.position + dir2 * diff);
                kit.depth -= 1;
                double scale = kit.scale;
                kit.scale = scale2;
                result += getColorOf(pos2, dir2, kit) * reflective;
                kit.scale = scale;
                kit.depth += 1;
            }
        }
#endif
    }
    result += surf.reflection * surf.self_lighting * (1 - reflective);
    //lights
    if (surf.reflective != 1) {
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
                result += c.reflect(surf.reflection) * (power * (1 - surf.reflective));
            }
        }
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
