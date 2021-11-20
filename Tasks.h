#pragma once
typedef void* HANDLE;
class Tasks
{
private:
	HANDLE* handles;
	int count;
public:
	Tasks();
	Tasks(int count);
	~Tasks();
	void runTask(void func(void*), void* args);
	void waitAll();
};

