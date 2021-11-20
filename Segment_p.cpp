#include "Segment_p.h"
Segment_p::Segment_p(SPoint s, SPoint e) :
	s(s), e(e) {}
Segment_p::Segment_p(const Segment_p& oth) :
	s(oth.s), e(oth.e) {}
Segment_p::Segment_p(double s, double e, ObjectModel* om):
	s(om, s), e(om, e) {}
Segment_p& Segment_p::operator=(const Segment_p& oth) {
	this->s = oth.s;
	this->e = oth.e;
	return *this;
}