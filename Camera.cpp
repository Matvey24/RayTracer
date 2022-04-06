#include "Camera.h"
#include "strsafe.h"
#include "tchar.h"
#include "windows.h"
RTL_CRITICAL_SECTION* sect;
Camera::Camera(Scene& scene, size_t width, size_t height):scene(scene), image(NULL) {
	rot.setPrimary();
	FOV = 60;
	max_depth = 20;
	resetTable(width, height);
	sect = new RTL_CRITICAL_SECTION;
	InitializeCriticalSection(sect);
}
Camera::~Camera() {
	delete(image);
	DeleteCriticalSection(sect);
	delete(sect);
}
struct THREAD_DATA {
	void (*func)(THREAD_DATA&);
	Camera* c;
	size_t line;
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
void executeParallel(Camera *cam, void func(THREAD_DATA&)) {
#ifdef _DEBUG
	const int tc = 1;
#else
	int num = 4;
	{
		//char* chars;
		//size_t count = 1;
		//_getenv_s(&chars, &count, "NUMBER_OF_PROCESSORS");
		//sscanf(chars, "%d", &num);
		//free(chars);
	}
	const int tc = num;
#endif
	THREAD_DATA* td = (THREAD_DATA*)malloc(sizeof(THREAD_DATA) * tc);
	HANDLE* h = (HANDLE*)malloc(sizeof(HANDLE) * tc);
	if (h == NULL) {
		if (td != NULL)
			free(td);
		exit(1);
	}
	for (size_t i = 0; i < tc; ++i) {
		td[i].c = cam;
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
	work_count = image->width;
	work_id = 0;
	executeParallel(this, runRender);
}
void Camera::render360() {
	work_count = image->width;
	work_id = 0;
	executeParallel(this, runRender360);
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
int Camera::getWork(THREAD_DATA& td) {
	EnterCriticalSection(sect);
	td.line = work_id;
	work_id++;
	LeaveCriticalSection(sect);
	return work_id <= work_count;
}
void runRender(THREAD_DATA &td) {
	Camera& c = *td.c;
	DevelopmentKit kit;
	kit.scene = &c.scene;
	ImageBMP* im = c.image;
	size_t width = im->width, height = im->height;
	while (c.getWork(td)) {
		double dirx = width / 2. - td.line;
		for (size_t y = 0; y < height; ++y) {
			Vector3 pos(c.pos);
			Vector3 dir(c.table_dist, y - height / 2., dirx);
			c.rot.transformBack(dir);
			kit.depth = c.max_depth;
			kit.scale = 1;
			Color col = c.scene.getColorOf(pos, dir, kit);
			im->set((unsigned)td.line, (unsigned)y, col.toRGB());
		}
	}
}
void runRender360(THREAD_DATA &td) {
	Camera& c = *td.c;
	DevelopmentKit kit;
	kit.scene = &c.scene;
	Matrix m;
	size_t width = c.image->width, height = c.image->height;

	size_t start_line = td.line;
	size_t end_line = 0;
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
			m.setRotY(Vector2(2 * M_PI * (x + (d / 2.)) / width));
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