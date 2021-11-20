#include "Scene.h"
Scene::Scene() {
	spaceColor = Color(0.4, 0.4, 1);
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
Color Scene::getColorOf(Vector3& pos, Vector3& dir, size_t depth, double scale, DevelopmentKit &kit) const {
	dir.norm();
    SurfacePoint surf = intersect(pos, dir, kit);
    if(!surf.intersects)
	    return spaceColor;
    if (depth == 0)
        surf.reflective = 0;
    surf.norm.norm();
    Color result;
    double scl = dir.scl(surf.norm);
    //reflections
    if (surf.reflective != 0) {
        double reflective = pow(surf.reflective, (scl < 0) ? -scl : scl);
        double scale2 = scale * reflective;
        if (scale2 >= 0.01) {
            Vector3 dir2(dir - surf.norm * (2 * scl));
            Vector3 pos2(surf.position + dir2 * diff);
            result += getColorOf(pos2, dir2, depth - 1, scale2, kit) * reflective;
        }
    }
#ifdef FULL_REFLECTIONS
    if(surf.reflective != 1){
        int discretization = 5;
        double newscale = scale / discretization / 2 / discretization;

        Matrix mat;
        Vector3 dir2(dir - surf.norm * (2 * scl));
        for (int i = 0, m = discretization * 2; i < m; ++i) {
            Matrix vert_mat;
            vert_mat.setRotZ(Vector2(M_PI * i / m));
            for (int j = 0; j < discretization; ++j) {
                mat.setRotY(Vector2(2 * M_PI * j / discretization));
                mat = mat * vert_mat;
                Vector3 dir3(dir2);
                mat.transform(dir3);
                Vector3 pos2(surf.position + dir3 * diff);
                result += getColorOf(pos2, dir3, depth - 1, newscale, kit);
                
            }
        }
    }
#else
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
        result += surf.reflection * env_light_pow * (1 - surf.reflective);
    }
#endif
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
        return sp.model->getPoint(pos, dir, sp.pos);
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
