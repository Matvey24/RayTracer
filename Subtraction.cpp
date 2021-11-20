#include "Subtraction.h"
Subtraction::Subtraction(size_t count, OperatorModel* a[]) :
	models_count(count) {
	for (size_t i = 0; i < count; ++i)
		models[i] = a[i];
}
Segments* Subtraction::intersectAll(const Vector3& pos, const Vector3& dir, DevelopmentKit& kit) {
	Segments* main = models[0]->intersectAll(pos, dir, kit);
	Segments* tmp = kit.getSegments();
	for (size_t i = 1; i < models_count; ++i) {
		Segments* b_ = models[i]->intersectAll(pos, dir, kit);
		main->subtract(*b_, *tmp);
		kit.pushSegment(b_);
		b_ = main;
		main = tmp;
		tmp = b_;
		if (main->isEmpty())
			break;
		tmp->clear();
	}
	kit.pushSegment(tmp);
	return main;
}
void Subtraction::translate(const Vector3& vec) {
	for (size_t i = 0; i < models_count; ++i)
		models[i]->translate(vec);
}
void Subtraction::rotate(const Vector3& at, const Matrix& m) {
	for (size_t i = 0; i < models_count; ++i)
		models[i]->rotate(at, m);
}
