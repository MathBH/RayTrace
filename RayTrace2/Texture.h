#pragma once
#include "ParameterTypes.h"
#include <string>

class UVCoord {
public:
	double U;
	double V;
	UVCoord(double u, double v) : U(u), V(v) {}
	~UVCoord() {}
};

class Texture {
public:
	virtual ColorRGB getValueAt(UVCoord coord) = 0;
};