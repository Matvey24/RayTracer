#pragma once
#ifndef _OBJECT_
#define _OBJECT_
#include "OperatorModel.h"
#include "DevelopmentKit.h"
class ObjectModel: public OperatorModel {
public:
    Vector3 center;
    Matrix rot;
    MaterialModel *mm;
    ObjectModel(double cx, double cy, double cz, MaterialModel* mm);
    SurfacePoint getPoint(const Vector3& pos, const Vector3& dir, const double dist);
    SurfacePoint intersect(const Vector3& pos, const Vector3& dir, DevelopmentKit& kit);
    void translate(const Vector3& vec);
    void rotate(const Vector3& at, const Matrix& m);
    double getDist(double t1, double t2);
    virtual void getNorm(Vector3& internal) = 0;
};
SPoint getNearestModel(OperatorModel** models, size_t count,
    const Vector3& pos, const Vector3& dir, DevelopmentKit& kit, OperatorModel** model);
#endif