#pragma once
class ObjectModel;
class SPoint
{
public:
	ObjectModel*model;
	double pos;
	SPoint(ObjectModel* model, double pos);
	SPoint(const SPoint& oth);
	SPoint& operator=(const SPoint& oth);
};