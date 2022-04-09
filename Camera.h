#pragma once
#include"Scene.h"
#include"ImageBMP.h"
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
	double table_dist;
	ImageBMP* image;
	size_t work_id;
	size_t work_count;
public:
	Camera(Scene& scene, size_t width, size_t height);
	~Camera();
	void render();
	void render360();
	void renderVideo(int frames, const char file_format[], void update());
	void save(FILE* file) const;
	void save(const char* file_name) const;
	void resetTable(size_t width, size_t height);
	void rotate(const Vector3& at, const Matrix& m);
	int getWork(THREAD_DATA &td);
private:
	friend void runRender(THREAD_DATA &td);
	friend void runRender360(THREAD_DATA &td);
};


