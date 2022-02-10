#pragma once
#ifndef _DEVELOPMENT_KIT_
#define _DEVELOPMENT_KIT_
#include"Segments.h"
class Scene;
class DevelopmentKit
{
private:
	Segments** seg_stack;
	size_t seg_pos;
	size_t seg_cap;
public:
	Scene* scene;
	size_t depth;
	double scale;
	DevelopmentKit();
	~DevelopmentKit();
	Segments* getSegments();
	void pushSegment(Segments* seg);
};
#endif
