#pragma once
#include "Segment_p.h"
#include <stdlib.h>
class Segments
{
private:
	Segment_p* s;
	size_t s_len, s_cap;
public:
	Segments();
	~Segments();
	void unio(Segments& oth, Segments& saveTo);
	void intersect(Segments& oth, Segments& saveTo);
	void subtract(Segments& oth, Segments& saveTo);
	void clear();
	void makeOperation(Segments& oth, bool func(bool, bool), Segments& saveTo);
	bool isEmpty();
	void add(Segment_p& p);
	SPoint* getFirstActive();
	SPoint* getFirst();
};

