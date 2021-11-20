#include<stdio.h>
#include<ctime>
#include "Camera.h"
#include "Wall.h"
#include "Rectangle.h"
#include "Pyramid.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "ColorMaterial.h"
#include "Mirror.h"
#include "Tetrahedron.h"
#include "Subtraction.h"

void makePicture() {
    Matrix mat;
    Vector3 pos;

    ColorMaterial sphmm(0xff0000);
    ColorMaterial floamm(0xffff00);
    ColorMaterial rtmm(0x0000ff);
    ColorMaterial prmmm(0x00ff00);
    ColorMaterial starmm(0xFF5A23);
    Mirror m;

    HalfVolume flo(0, -1.5, 0, &floamm);

    Tetrahedron st(10, 1/(double)4, 3, 1, &starmm);
    Tetrahedron st2(10, 0, 3, 1, &starmm);
    mat.setRotZ(Vector2(-1, 0));
    pos.set(10, 0, 0);
    st2.rotate(pos, mat);
    Sphere sp(10, 0, 1, 1, &sphmm);
    Rectangle rect(10, 0, -1, 1, 1, 1, &rtmm);
    Pyramid pr(10, 0, -3, 1, 1, 1, &prmmm);
    HalfVolume mirr(16, 0, 0, &m);
    flo.rot.setRotZ(Vector2(0, 1));

    Rectangle r1(5, 3, 1, 1, 1, 1, &sphmm);
    Sphere s1(5, 3, 1, 1.4, &prmmm);
    OperatorModel* a[] = { &r1, &s1 };
    Subtraction sbtr1(2, a);
    DirectionalLight pl(-2, 5, 2, 1, Color(0xffffff));

    Scene scene;
    Camera camera(scene, 1920, 1080);
    //camera.rot.setRotY(Vector2(-1, 0));
    camera.pos = Vector3(7,3,0);
    scene.addModel(sp);
    scene.addModel(flo);
    scene.addModel(rect);
    scene.addModel(pr);
    scene.addModel(mirr);
    scene.addModel(st);
    scene.addModel(st2);
    scene.addModel(sbtr1);

    scene.addLight(pl);
    
    double delta = 2 * M_PI / (double)60 / 3;
    int pictures = 3 * 60 - 1;
    Vector2 d_ang(delta);
    
    for(int i = 0; i < pictures; ++i){
        unsigned t = clock();
        camera.render360();
        unsigned t2 = clock();
        char arr[20];
        sprintf_s(arr, "images\\img%d.bmp", i);
        FILE* f;
        fopen_s(&f, arr, "wb");
        if (f == NULL) {
            printf("Couldn't create file\n");
            exit(1);
        }
        camera.save(f);
        fclose(f);
        unsigned t3 = clock();
        printf("Image %d from %d rendered in %u, saved in %u millis\n", i, pictures, t2 - t, t3 - t2);
        
        Vector2 pos(camera.pos.x - 5, camera.pos.z - 1);
        pos.rotate(d_ang);
        camera.pos.set(pos.x + 5, camera.pos.y, pos.y + 1);
    }
}
int main()
{
    makePicture();
    
    return 0;
}