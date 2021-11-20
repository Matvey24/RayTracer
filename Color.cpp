#include "Color.h"
Color::Color(unsigned c) {
	RGB col;
	col.rgb = c;
	b = col.comps[0] / (double)255;
	g = col.comps[1] / (double)255;
	r = col.comps[2] / (double)255;
}
Color Color::reflect(Color oth) const{
	return Color(r * oth.r, g * oth.g, b * oth.b);
}
bool Color::isZero() const {
	return b == 0 && g == 0 && r == 0;
}
double fmin(double a, double b) {
	return a > b ? b : a;
}
int Color::toRGB() const {
	RGB c;
	c.comps[0] = (unsigned char)(fmin(b, 1) * 255);
	c.comps[1] = (unsigned char)(fmin(g, 1) * 255);
	c.comps[2] = (unsigned char)(fmin(r, 1) * 255);
	c.comps[3] = 0;
	return c.rgb;
}
Color operator+(Color a, Color b) {
	return Color(a.r + b.r, a.g + b.g, a.b + b.b);
}
Color& operator+=(Color& a, Color b) {
	a.r += b.r;
	a.g += b.g;
	a.b += b.b;
	return a;
}
Color operator*(Color a, double b) {
	return Color(a.r * b, a.g * b, a.b * b);
}
Color operator/(Color a, double v) {
	double t = 1 / v;
	return Color(a.r * t, a.g * t, a.b * t);
}
Color& operator/=(Color& a, double v) {
	double t = 1 / v;
	a.r *= t;
	a.g *= t;
	a.b *= t;
	return a;
}