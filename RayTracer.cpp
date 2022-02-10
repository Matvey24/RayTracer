#include<ctime>
#include "Camera.h"
#include "Wall.h"
#include "Pyramid.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "ColorMaterial.h"
#include "Mirror.h"
#include "Tetrahedron.h"
#include "Subtraction.h"
#include "RectTexture.h"
#include "SphereTexture.h"
//#include "Thor.h"
//#include "ThorTexture.h"
#include "Rectangle.h"
#include "DoubleTexturedSphere.h"
#include "HardBody.h"
void makePicture() {
    printf("Loading textures..\n");
    ImageBMP* earth_i = new ImageBMP("textures/8k_earth.bmp");
    printf("Done. Preparing..\n");
    Matrix mat;
    Vector3 pos;

    SphereTexture stext(earth_i);
    RectTexture rtext(earth_i);
    ColorMaterial floamm(0xffff01);
    ColorMaterial rtmm(0x0101ff);
    ColorMaterial prmmm(0x01ff01);
    ColorMaterial starmm(0xFF5A23);
    Mirror m;


    HalfVolume flo(0, -1.5, 0, &floamm);

    Tetrahedron st(10, 1/(double)4, 3, 1, &starmm);
    Tetrahedron st2(10, 0, 3, 1, &starmm);


    mat.setRotZ(Vector2(-1, 0));
    pos.set(10, 0, 0);
    st2.rotate(pos, mat);
    Sphere sp(10, 0, 1, 1, &stext);
    HardBody hbs(1, Vector3(2 / 3., 2 / 3., 2 / 3.), &sp);
    hbs.w.set(1, 1, 1);
    hbs.w.norm();

    Rectangle rect(10, 0, -1, 1, 1, 1, &stext);
    Pyramid pr(10, 0, -3, 1, 1, 1, &prmmm);
    //HalfVolume mirr(16, 0, 0, &m);
    flo.rot.setRotZ(Vector2(0, 1));

    DirectionalLight pl(-2, 5, 2, 1, Color(0xffffff));
    ColorMaterial space(Color(0.4, 0.4, 1).toRGB());
    Scene scene(&space);
    Camera camera(scene, 1360, 768);
    camera.rot.setRotZ(Vector2(M_PI / 8));
    camera.pos = Vector3(0, 3, 1);
    scene.addModel(sp);
    scene.addModel(flo);
    scene.addModel(rect);
    scene.addModel(pr);
    //scene.addModel(mirr);
    scene.addModel(st);
    scene.addModel(st2);
    scene.addLight(pl);
    
    int fps = 1;
    double seconds = 10;
    int pictures = (int)(seconds * fps);
    double delta = (2 * M_PI) / pictures;
    
    for(int i = 0; i < pictures; ++i){
        unsigned t = clock();
        camera.render();
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
        
        hbs.update(delta);
    }
    delete(earth_i);
}
void makePicture2() {
    printf("Loading textures..\n");
    //ImageBMP
        //*earth_t = new ImageBMP("textures/8k_earth.bmp"),
        //* space_t = new ImageBMP("textures/8k_space.bmp");
        //*earth_n  = new ImageBMP("textures/8k_earth_n.bmp");
    printf("Done. Preparing..\n");
    Matrix mat;
    Matrix mat2;
    Vector3 pos;
    Vector2 ang;
    
    //ThorTexture spt(earth_t, 1);
    //Thor sp(0, 0, 0, 0.4, 1, &spt);
    //SphereTexture space(space_t);
    ColorMaterial SKY(Color(0.4, 0.4, 1).toRGB());
    Scene s(&SKY);

    //SphereTexture eart_d(earth_t);
    //SphereTexture eart_n(earth_n);
    ColorMaterial GREEN(0x02ff02);
    ColorMaterial BLUE(0x0202ff);
    ColorMaterial YELLOW(0xffff02);

    Rectangle hor1(1, 1, 0, 0.5, 0.5, 2, &GREEN);
    Rectangle ver1(1, -0.5, 0, 0.5, 1, 0.5, &BLUE);
    OperatorModel* a1[2] = { &hor1, &ver1 };
    ComplexModel mod1(1, 0.5, 0, 10, 2, a1);
    HardBody hsph1(1, Vector3(225. / 48, 140. / 48, 103. / 48), &mod1);
    hsph1.w.set(0.01, 1, 0);

    Rectangle hor2(1, 1, 5, 0.5, 0.5, 2, &GREEN);
    Rectangle ver2(1, -0.5, 5, 0.5, 1, 0.5, &BLUE);
    OperatorModel* a2[2] = { &hor2, &ver2 };
    ComplexModel mod2(1, 0.5, 5, 10, 2, a2);
    HardBody hsph2(1, Vector3(225. / 48, 140. / 48, 103. / 48), &mod2);
    hsph2.w.set(0, 0.01, 1);

    Rectangle hor3(1, 1, -5, 0.5, 0.5, 2, &GREEN);
    Rectangle ver3(1, -0.5, -5, 0.5, 1, 0.5, &BLUE);
    OperatorModel* a3[2] = { &hor3, &ver3 };
    ComplexModel mod3(1, 0.5, -5, 10, 2, a3);
    HardBody hsph3(1, Vector3(225. / 48, 140. / 48, 103. / 48), &mod3);
    hsph3.w.set(1, 0.01, 0);

    //Sphere earth(0, 0, 0, 1, &eart_d);
    //hsph.w *= 0.3;
    
    HalfVolume floor(0, -4, 0, &YELLOW);
    floor.rot.setRotZ(Vector2(0, 1));
    DirectionalLight light(-2, 5, 2, 1, Color(0xffffff));
    s.env_light_pow = 0;
    Camera cam(s, 1920, 1080);

    Vector2 v(2, 1);
    v.norm();

    cam.pos.set(-12, 5, 0);
    cam.FOV = 60;
    //mat.setRotZ(v);
    cam.rot.setRotZ(Vector2(M_PI / 8));
    //cam.rot = cam.rot * mat;

    //s.addModel(sp);
    s.addModel(mod1);
    s.addModel(mod2);
    s.addModel(mod3);
    s.addModel(floor);
    s.addLight(light);
    
    int fps = 10;
    double seconds = 1;
    int pictures = (int)(seconds * fps);
    double delta = (18*M_PI) / fps;


    //Matrix mat3;
    //mat3.setRotZ(Vector2(M_PI / 4));
    //mat.setRotY(Vector2(2 * delta));
    //mat = mat * mat3;
    //mat2.setRotY(Vector2(-delta));


    for (int i = 0; i < pictures; ++i) {
        unsigned t = clock();
        cam.render();
        unsigned t2 = clock();
        char arr[20];
        sprintf_s(arr, "images\\img%d.bmp", i);
        cam.save(arr);

        unsigned t3 = clock();
        printf("Image %d from %d rendered in %u m, saved in %u m\n", i, pictures, t2 - t, t3 - t2);

        //mat2.transformBack(cam.pos);
        //cam.rot = cam.rot * mat2;
        //earth.rotate(Vector3(0, 0, 0), mat);
        for (int j = 0; j < 100; ++j) {
            hsph1.update(delta / 100);
            hsph2.update(delta / 100);
            hsph3.update(delta / 100);
        }
    }
    //delete(earth_t);
    //delete(space_t);
    //delete(earth_n);
}
int main()
{
    makePicture2();
    return 0;
}