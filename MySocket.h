#pragma once
class SOCKET;
class MySocket
{
private:
	SOCKET* s;
public:
	MySocket(int port);
	MySocket(char* arrdess, int port);
	void write(void* buf, size_t len);
	void flush();
	size_t read(void* buf, size_t len);
	void close();
};

