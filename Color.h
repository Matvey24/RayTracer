#pragma once
#ifndef _COLOR_
#define _COLOR_
class Color
{
public:
	double r = 0, g = 0, b = 0;
	Color(){}
	Color(unsigned c);
	Color(double r, double g, double b): r(r), g(g), b(b){}
	Color reflect(Color oth) const;
	int toRGB() const;
	bool isZero() const;
};
union RGB {
	int rgb;
	unsigned char comps[4];
};
Color operator+(Color a, Color b);
Color& operator+=(Color& a, Color b);
Color operator*(Color a, double v);
Color operator/(Color a, double v);
Color& operator/=(Color& a, double v);
#endif
