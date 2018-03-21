#pragma once
#define ZERO_VAL 0.0

class ColorRGB {
public:
	double R, G, B;
	ColorRGB() : R(ZERO_VAL), G(ZERO_VAL), B(ZERO_VAL) {}
	ColorRGB(double r, double g, double b) : R(r), G(g), B(b) {}
	~ColorRGB() {}
};