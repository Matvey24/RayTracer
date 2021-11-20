#pragma once
#include"Scene.h"
#include<stdio.h>
struct THREAD_DATA;
void runRender(THREAD_DATA &td);
void runRender360(THREAD_DATA &td);
class Camera
{
public:
	Scene& scene;
	Matrix rot;
	Vector3 pos;
	double FOV;
	size_t max_depth;
private:
	size_t width, height;
	double table_dist;
	int** table = NULL;
	char* bmp_buf;
public:
	Camera(Scene& scene, size_t width, size_t height);
	~Camera();
	void render();
	void render360();
	void save(FILE* file);
	void resetTable(size_t width, size_t height);
private:
	friend void runRender(THREAD_DATA &td);
	friend void runRender360(THREAD_DATA &td);
};


