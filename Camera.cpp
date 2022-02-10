#include "Camera.h"
#include "strsafe.h"
#include "tchar.h"
#include "windows.h"
Camera::Camera(Scene& scene, size_t width, size_t height):scene(scene), image(NULL) {
	rot.setPrimary();
	FOV = 60;
	max_depth = 100;
	resetTable(width, height);
}
Camera::~Camera() {
	delete(image);
}
struct THREAD_DATA {
	void (*func)(THREAD_DATA&);
	Camera* c;
	size_t start, end;
};
DWORD WINAPI worker(LPVOID args) {
	THREAD_DATA td = *((THREAD_DATA*)args);
	td.func(td);
	return 0;
}
void Camera::rotate(const Vector3& at, const Matrix& m) {
	rot = rot * m;
	Vector3 tmp(at + pos);
	m.transformBack(tmp);
	pos = tmp + at;
}
void executeParallel(Camera *cam, void func(THREAD_DATA&), size_t size) {
	const int tc = 4;
	THREAD_DATA* td = (THREAD_DATA*)malloc(sizeof(THREAD_DATA) * tc);
	HANDLE* h = (HANDLE*)malloc(sizeof(HANDLE) * tc);
	if (h == NULL) {
		if (td != NULL)
			free(td);
		exit(1);
	}
	for (size_t i = 0; i < tc; ++i) {
		td[i].c = cam;
		td[i].start = size * i / tc;
		td[i].end = size * (i + 1) / tc;
		td[i].func = func;
		DWORD dw;
		h[i] = CreateThread(
			NULL,
			0,
			worker,
			&td[i],
			0,
			&dw
		);
	}
	WaitForMultipleObjects(tc, h, TRUE, INFINITE);
	for (size_t i = 0; i < tc; ++i)
		CloseHandle(h[i]);
	free(td);
	free(h);
}
void Camera::render() {
	table_dist = image->width / tan(FOV / 360 * M_PI) / 2;
	executeParallel(this, runRender, image->width * image->height);
}
void Camera::render360() {
	executeParallel(this, runRender360, image->height);
}
void Camera::save(FILE* file) const {
	image->save(file);
}
void Camera::save(const char* file_name) const {
	FILE* file;
	fopen_s(&file, file_name, "wb");
	if (file == NULL) {
		printf("Error saving to file %s\n", file_name);
		exit(1);
	}
	image->save(file);
	fclose(file);
}
void Camera::resetTable(size_t width, size_t height) {
	if (image != NULL)
		delete(image);
	image = new ImageBMP(width, height);
}
void runRender(THREAD_DATA &td) {
	Camera& c = *td.c;
	size_t start_pixel = td.start, end_pixel = td.end;
	DevelopmentKit kit;
	kit.scene = &c.scene;
	size_t width = c.image->width, height = c.image->height;
	ImageBMP* im = c.image;
	for (size_t i = start_pixel; i < end_pixel; ++i) {
		size_t y = i / width;
		size_t x = i % width;
		Vector3 pos(c.pos);
		Vector3 dir(c.table_dist, height / (double)2 - y, width / (double)2 - x);
		c.rot.transformBack(dir);
		kit.depth = c.max_depth;
		kit.scale = 1;
		Color col = c.scene.getColorOf(pos, dir, kit);
		im->set((unsigned)x, (unsigned)y, col.toRGB());
	}
}
void runRender360(THREAD_DATA &td) {
	Camera& c = *td.c;
	size_t start_line = td.start;
	size_t end_line = td.end;
	DevelopmentKit kit;
	Matrix m;
	size_t width = c.image->width;
	size_t height = c.image->height;
	for (size_t y = start_line; y < end_line; ++y) {
		Vector2 vert_ang(M_PI * y / height);
		size_t d = 1;
		if (vert_ang.y == 0)
			d = width;
		else {
			double sin = vert_ang.y;
			while (sin < 0.5 && d < width) {
				sin *= 2;
				d *= 2;
			}
		}
		vert_ang.set(-vert_ang.y, vert_ang.x);
		for (size_t x = 0; x < width; x += d) {
			Vector3 dir(vert_ang.x, vert_ang.y, 0);
			m.setRotY(Vector2(2 * M_PI * (x + (d / (double)2)) / width));
			m.transform(dir);
			Vector3 pos(c.pos);
			c.rot.transformBack(dir);
			kit.depth = c.max_depth;
			kit.scale = 1;
			Color col = c.scene.getColorOf(pos, dir, kit);
			unsigned rgb = col.toRGB();
			for (size_t i = 0, k = ((d > width - x) ? width - x : d); i < k; ++i)
				c.image->set((unsigned)(x + i), (unsigned)y, rgb);
		}
	}
}