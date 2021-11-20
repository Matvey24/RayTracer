#include "Tasks.h"
#include <windows.h>
#include <tchar.h>
Tasks::Tasks():Tasks(1) {}
Tasks::Tasks(int count) {
	handles = (HANDLE*)malloc(sizeof(HANDLE) * count);
	if (handles == NULL) {
		exit(1);
		return;
	}
	count = 0;
}
struct myfunc {
	void (*func)(void*);
	void* args;
};
Tasks::~Tasks() {
	free(handles);
}
DWORD WINAPI mythreadfunc(LPVOID lpvoid) {
	myfunc* mf = (myfunc*)lpvoid;
	mf->func(mf->args);
	return 0;
}
void Tasks::runTask(void func(void*), void* args) {
	DWORD i;
	myfunc arg;
	arg.func = func;
	arg.args = args;
	handles[count++] = CreateThread(
		NULL,
		0,
		mythreadfunc,
		&arg,
		0,
		&i
	);
}
void Tasks::waitAll() {
	WaitForMultipleObjects(count, handles, TRUE, INFINITE);
	for (int i = 0; i < count; ++i) {
		CloseHandle(handles[i]);
	}
	count = 0;
}
