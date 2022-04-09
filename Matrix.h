#pragma once
#ifndef _UTILS_
#define _UTILS_
#include "Vector3.h"
class Matrix
{
public:
	double a1 = 0, a2 = 0, a3 = 0, b1 = 0, b2 = 0, b3 = 0, c1 = 0, c2 = 0, c3 = 0;
	Matrix() {}
	Matrix(const Matrix& oth);
	Matrix(double a1, double a2, double a3,
		double b1, double b2, double b3,
		double c1, double c2, double c3);
	double det() const;
	void mult(double val);
	void transform(Vector3& vec) const;
	void transformBack(Vector3& vec) const;
	const Matrix& set(double a1, double a2, double a3,
		double b1, double b2, double b3,
		double c1, double c2, double c3);
	const Matrix& set(const Matrix& oth);
	const Matrix& setPrimary();
	const Matrix& setRotX(Vector2 ang);
	const Matrix& setRotY(Vector2 ang);
	const Matrix& setRotZ(Vector2 ang);
	const Matrix& setRotE(Vector3 at, Vector2 ang);

	void setRotOf(Vector3 ang);
	void reverse();
};
Matrix operator*(Matrix a, Matrix b);
Matrix operator-(Matrix a, Matrix b);
#endif