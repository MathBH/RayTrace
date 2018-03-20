#pragma once
/*
	Color container for RGB

	values between 0 and 1
*/
class ColorRGB {
public:
	double R, G, B;
	ColorRGB(double r, double g, double b) : R(r), G(g), B(b) {}
	~ColorRGB(){}
};