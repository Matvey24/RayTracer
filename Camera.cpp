#include "Camera.h"
#include "strsafe.h"
#include "tchar.h"
#include "windows.h"
Camera::Camera(Scene& scene, size_t width, size_t height):scene(scene) {
	rot.setPrimary();
	FOV = 60;
	max_depth = 100;
	resetTable(width, height);
}
Camera::~Camera() {
	for (size_t i = 0; i < this->height; ++i)
		free(table[i]);
	free(table);
	free(bmp_buf);
	table = NULL;
	bmp_buf = NULL;
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
	table_dist = width / tan(FOV / 360 * M_PI) / 4;
	executeParallel(this, runRender, width * height);
}
void Camera::render360() {
	executeParallel(this, runRender360, height);
}
void bwrite(char* to, void* buf, size_t el_size, size_t el_count, size_t& idx) {
	char* buf2 = (char*)buf;
	for (size_t i = 0, m = el_size * el_count; i < m; ++i) {
		to[idx] = buf2[i];
		idx++;
	}
}
void Camera::save(FILE* file) {
	unsigned l = (4 - (width * 3) % 4) % 4;
	unsigned len = 14 + 40 + (3 * width + l) * height;
	size_t idx = 0;

	char a[] = "BM";
	bwrite(bmp_buf, a, 1, 2, idx);
	bwrite(bmp_buf, &len, 4, 1, idx);
	char b[] = {0, 0, 0, 0};
	bwrite(bmp_buf, b, 1, 4, idx);
	unsigned offset = 14 + 40;//
	bwrite(bmp_buf, &offset, 4, 1, idx);

	unsigned head = 40;
	bwrite(bmp_buf, &head, 4, 1, idx);
	bwrite(bmp_buf, &width, 4, 1, idx);
	bwrite(bmp_buf, &height, 4, 1, idx);
	unsigned short planes = 1;
	bwrite(bmp_buf, &planes, 2, 1, idx);
	unsigned short bit_count = 24;
	bwrite(bmp_buf, &bit_count, 2, 1, idx);
	unsigned compression = 0;
	bwrite(bmp_buf, &compression, 4, 1, idx);
	unsigned size = 0;
	bwrite(bmp_buf, &size, 4, 1, idx);
	unsigned scale = 3780;
	bwrite(bmp_buf, &scale, 4, 1, idx);
	bwrite(bmp_buf, &scale, 4, 1, idx);
	bwrite(bmp_buf, &compression, 4, 1, idx);
	bwrite(bmp_buf, &compression, 4, 1, idx);

	for (size_t y = 0; y < height; ++y) {
		size_t cy = height - y - 1;
		for (size_t x = 0; x < width; ++x)
			bwrite(bmp_buf, &table[cy][x], 3, 1, idx);
		bwrite(bmp_buf, b, 1, l, idx);
	}
	fwrite(bmp_buf, 1, idx, file);
}
void Camera::resetTable(size_t width, size_t height) {
	if (table != NULL) {
		this->~Camera();
	}
	this->width = width;
	this->height = height;
	unsigned l = (4 - (width * 3) % 4) % 4;
	unsigned len = 14 + 40 + (3 * width + l) * height;
	bmp_buf = (char*)malloc(len);
	if (bmp_buf == NULL)
		exit(1);
	table = (int**)malloc(sizeof(int*) * height);
	if (table == NULL) {
		exit(1);
		return;
	}
	for (size_t i = 0; i < height; ++i)
		table[i] = (int*)malloc(sizeof(int) * width);
	if (table[height - 1] == NULL)
		exit(1);
}
void runRender(THREAD_DATA &td) {
	Camera& c = *td.c;
	size_t start_pixel = td.start, end_pixel = td.end;
	DevelopmentKit kit;
	size_t width = c.width, height = c.height;
	for (size_t i = start_pixel; i < end_pixel; ++i) {
		size_t y = i / width;
		size_t x = i % width;
		Vector3 pos(c.pos);
		Vector3 dir(c.table_dist, height / (double)2 - y, width / (double)2 - x);
		c.rot.transformBack(dir);
		Color col = c.scene.getColorOf(pos, dir, c.max_depth, 1, kit);
		c.table[y][x] = col.toRGB();
	}
}
void runRender360(THREAD_DATA &td) {
	Camera& c = *td.c;
	size_t start_line = td.start;
	size_t end_line = td.end;
	DevelopmentKit kit;
	Matrix m;
	for (size_t y = start_line; y < end_line; ++y) {
		Vector2 vert_ang(M_PI * y / c.height);
		size_t d = 1;
		if (vert_ang.y == 0)
			d = c.width;
		else {
			double sin = vert_ang.y;
			while (sin < 0.5 && d < c.width) {
				sin *= 2;
				d *= 2;
			}
		}
		vert_ang.set(-vert_ang.y, vert_ang.x);
		for (size_t x = 0; x < c.width; x += d) {
			Vector3 dir(vert_ang.x, vert_ang.y, 0);
			m.setRotY(Vector2(2 * M_PI * (x + (d / (double)2)) / c.width));
			m.transform(dir);
			Vector3 pos(c.pos);
			c.rot.transformBack(dir);
			Color col = c.scene.getColorOf(pos, dir, c.max_depth, 1, kit);
			int rgb = col.toRGB();
			for (size_t i = 0, k = ((d > c.width - x) ? c.width - x: d); i < k; ++i)
				c.table[y][x + i] = rgb;
		}
	}
}