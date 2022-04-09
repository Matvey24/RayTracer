
#include "Camera.h"
#include "Material.h"
#include "Texture.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "Wall.h"
#include "Pyramid.h"
#include "Tetrahedron.h"
#include "Subtraction.h"
#include "MandelBulb.h"
//#include "Thor.h"
//#include "ThorTexture.h"
#include "Rectangle.h"
#include "HardBody.h"

void makePicture() {
    printf("Loading textures..\n");
    ImageBMP* meet_i = new ImageBMP("textures/meet3.bmp");
    printf("Done. Preparing..\n");
    Matrix mat;
    Vector3 pos;

    //RectTexture stext(meet_i);
    Texture meet = Textures::makeSpherical(meet_i);

    Material rtext = Materials::defColor(0x0000ff);
    Material floamm = Materials::defColor(0xffff01);
    Material rtmm = Materials::defColor(0x8181ff);
    Material prmmm = Materials::defColor(0x01ff01);
    Material starmm = Materials::defColor(0xFF5A23);
    Material space = Materials::defColor(0xffffff);

    Material m = Materials::mirror();


    HalfVolume flo(0, -1.5, 0, &floamm);

    Sphere st(7, 0, 0, 1, &meet);
    mat.setRotZ(Vector2(-1, 0));
    pos.set(10, 0, 0);
    flo.rot.setRotZ(Vector2(0, 1));

    DirectionalLight pl(-2, 5, 2, 0.7, 0xffffff);
    Scene scene(&space);
    Camera camera(scene, 1024, 1024);
    camera.rot.setRotZ(Vector2(M_PI / 7));
    camera.pos = Vector3(5, 1, 0);
    scene.addModel(flo);
    scene.addModel(st);
    scene.addLight(pl);

    int fps = 1;
    double seconds = 1;
    int pictures = (int)(seconds * fps);
    double delta = (2 * M_PI) / pictures;

    camera.renderVideo(1, "images/img%d.bmp", []() {});
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
    Material SKY = Materials::defColor(Color(0.4, 0.4, 1).toRGB());
    Scene s(&SKY);

    //SphereTexture eart_d(earth_t);
    //SphereTexture eart_n(earth_n);
    Material GREEN = Materials::defColor(0x02ff02);
    Material BLUE = Materials::defColor(0x0202ff);
    Material YELLOW = Materials::defColor(0xffff02);

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

    cam.renderVideo(pictures, "images\\img%d.bmp",
        []() {});
            //for (int j = 0; j < 100; ++j) {
            //    hsph1.update(delta / 100);
            //    hsph2.update(delta / 100);
            //    hsph3.update(delta / 100);
            //}
        //});
    //delete(earth_t);
    //delete(space_t);
    //delete(earth_n);
}
void carnellBox() {
    printf("Prepairing...\n");
    Material orange(0, 0xFF5A23);
    Material blue(0, 0x0000FF);
    Material white(0, 0xFFFFFF);
    Material black = Materials::defColor(0);
    Material lighting = Materials::light(0xFFFFFF, 10);

    Matrix m;
    HalfVolume floor(0, 0, 0, &white);
    floor.rot.setRotZ(Vector2(M_PI_2));
    HalfVolume top(0, 6, 0, &white);
    top.rot.setRotZ(Vector2(-M_PI_2));
    HalfVolume forwWall(4, 0, 0, &white);
    //forwWall.rot.setRotY(Vector2(M_PI));
    HalfVolume leftWall(0, 0, 4, &orange);
    leftWall.rot.setRotY(Vector2(M_PI_2));
    HalfVolume rightWall(0, 0, -4, &blue);
    rightWall.rot.setRotY(Vector2(-M_PI_2));
    HalfVolume backWall(-4, 0, 0, &white);
    backWall.rot.setRotY(Vector2(M_PI));
    Sphere splight(0, 7, 0, 1.3, &lighting);


    //PointLight light(1, 2, 0, 2, Color(0xffffff));
    Scene sc(&white);
    //sc.addLight(light);

    sc.addModel(floor);
    sc.addModel(top);
    sc.addModel(forwWall);
    sc.addModel(leftWall);
    sc.addModel(rightWall);
    sc.addModel(backWall);
    sc.addModel(splight);
    Camera cam(sc, 1360, 768);
    cam.FOV = 100;
    cam.pos.set(-3.5, 2.5, 0);


    int fps = 1;
    double seconds = 1;
    int pictures = (int)(seconds * fps);
    double delta = (2 * M_PI) / pictures;

    cam.renderVideo(pictures, "images\\img%d.bmp", []() {});
}
void GPUcomparing() {
    Material sph(0.2, 0xff0000);
    Material fl(0.2, 0x00ffff);
    Material sky(0, 0xffffff);

    Sphere sp(4, 0, 0, 1, &sph);
    Sphere floor(0, -100, 0, 99, &fl);

    Scene scene(&sky);
    scene.addModel(sp);
    scene.addModel(floor);

    Camera cam(scene, 1280, 720);
    cam.rot.setRotZ(Vector2(M_PI / 8));
    cam.pos.set(0, 1, 0);
    
    cam.renderVideo(1, "images\\img%d.bmp", []() {});
}
int main()
{
    makePicture();
    return 0;
}