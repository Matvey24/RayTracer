#include "SPoint.h"
SPoint::SPoint(ObjectModel* model, double pos):
	model(model), pos(pos) {}
SPoint::SPoint(const SPoint& oth):
model(oth.model), pos(oth.pos){}

SPoint& SPoint::operator=(const SPoint& oth) {
	this->pos = oth.pos;
	this->model = oth.model;
	return *this;
}