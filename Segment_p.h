#pragma once
#include"SPoint.h"
class Segment_p
{
public:
	SPoint s, e;
	Segment_p(SPoint s, SPoint e);
	Segment_p(const Segment_p& oth);
	Segment_p(double s, double e, ObjectModel* om);
	Segment_p& operator=(const Segment_p& oth);
};