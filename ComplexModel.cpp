#include "ComplexModel.h"
ComplexModel::ComplexModel(double x, double y, double z, double rad, size_t count, OperatorModel* a[]):
sp(x, y, z, rad, NULL), model_count(count){
	for (size_t i = 0; i < count; ++i)
		models[i] = a[i];
}
ComplexModel::ComplexModel(double x, double y, double z, double rad, OperatorModel* a) :
	sp(x, y, z, rad, NULL), model_count(1) {
	models[0] = a;
}
Segments* ComplexModel::intersectAll(const Vector3& pos, const Vector3& dir, DevelopmentKit& kit) {
	SPoint dist = sp.getDistance(pos, dir, kit);
	if (dist.pos != dist.pos)
		return kit.getSegments();
	OperatorModel* op;
	SPoint p = getNearestModel(models, model_count, pos, dir, kit, &op);
	if (p.model == NULL)
		return kit.getSegments();
	return op->intersectAll(pos, dir, kit);
}
SPoint ComplexModel::getDistance(const Vector3& pos, const Vector3& dir, DevelopmentKit& kit) {
	SPoint dist = sp.getDistance(pos, dir, kit);
	if (dist.pos != dist.pos)
		return SPoint(NULL, NAN);
	OperatorModel* om;
	return getNearestModel(models, model_count, pos, dir, kit, &om);
}
void ComplexModel::translate(const Vector3& vec) {
	sp.translate(vec);
	for (size_t i = 0; i < model_count; ++i)
		models[i]->translate(vec);
}
void ComplexModel::rotate(const Vector3& at, const Matrix& m) {
	sp.rotate(at, m);
	for (size_t i = 0; i < model_count; ++i)
		models[i]->rotate(at, m);
}
Matrix ComplexModel::getRot() {
	return sp.rot;
}
Vector3 ComplexModel::getPos() {
	return sp.center;
}