#include "Matrix.h"
Matrix::Matrix(const Matrix& oth) :
    a1(oth.a1), a2(oth.a2), a3(oth.a3),
    b1(oth.b1), b2(oth.b2), b3(oth.b3),
    c1(oth.c1), c2(oth.c2), c3(oth.c3) {}
Matrix::Matrix(double a1, double a2, double a3,
    double b1, double b2, double b3,
    double c1, double c2, double c3) :
    a1(a1), a2(a2), a3(a3),
    b1(b1), b2(b2), b3(b3),
    c1(c1), c2(c2), c3(c3) {}
void Matrix::transform(Vector3& vec) const{
    double x, y, z;
    x = a1 * vec.x + a2 * vec.y + a3 * vec.z;
    y = b1 * vec.x + b2 * vec.y + b3 * vec.z;
    z = c1 * vec.x + c2 * vec.y + c3 * vec.z;
    vec.set(x, y, z);
}
void Matrix::transformBack(Vector3& vec) const{
    double x, y, z;
    x = a1 * vec.x + b1 * vec.y + c1 * vec.z;
    y = a2 * vec.x + b2 * vec.y + c2 * vec.z;
    z = a3 * vec.x + b3 * vec.y + c3 * vec.z;
    vec.set(x, y, z);
}
void Matrix::set(double a1, double a2, double a3,
    double b1, double b2, double b3,
    double c1, double c2, double c3) {
    this->a1 = a1;
    this->a2 = a2;
    this->a3 = a3;
    this->b1 = b1;
    this->b2 = b2;
    this->b3 = b3;
    this->c1 = c1;
    this->c2 = c2;
    this->c3 = c3;
}
void Matrix::setPrimary() {
    set(1, 0, 0, 0, 1, 0, 0, 0, 1);
}
void Matrix::setRotX(Vector2 ang) {
    set(1, 0, 0, 0, ang.x, -ang.y, 0, ang.y, ang.x);
}
void Matrix::setRotY(Vector2 ang) {
    set(ang.x, 0, ang.y, 0, 1, 0, -ang.y, 0, ang.x);
}
void Matrix::setRotZ(Vector2 ang) {
    set(ang.x, -ang.y, 0, ang.y, ang.x, 0, 0, 0, 1);
}
void Matrix::reverse() {
    double i = a2;
    a2 = b1;
    b1 = i;
    i = a3;
    a3 = c1;
    c1 = i;
    i = b3;
    b3 = c2;
    c2 = i;
}
Matrix operator*(Matrix a, Matrix b) {
    return Matrix(
        a.a1 * b.a1 + a.a2 * b.b1 + a.a3 * b.c1,
        a.a1 * b.a2 + a.a2 * b.b2 + a.a3 * b.c2,
        a.a1 * b.a3 + a.a2 * b.b3 + a.a3 * b.c3,
        a.b1 * b.a1 + a.b2 * b.b1 + a.b3 * b.c1,
        a.b1 * b.a2 + a.b2 * b.b2 + a.b3 * b.c2,
        a.b1 * b.a3 + a.b2 * b.b3 + a.b3 * b.c3,
        a.c1 * b.a1 + a.c2 * b.b1 + a.c3 * b.c1,
        a.c1 * b.a2 + a.c2 * b.b2 + a.c3 * b.c2,
        a.c1 * b.a3 + a.c2 * b.b3 + a.c3 * b.c3);
}