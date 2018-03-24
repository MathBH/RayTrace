#pragma once
#include "ParameterTypes.h"
#define DEFAULT_RTMAT_COLOR ColorRGB(1.0,1.0,1.0)
#define DEFAULT_RTMAT_REFLECTIVITY 0.5
/*
	Material compatible with ray tracer
*/
class RTMaterial {
private:
	ColorRGB color;
	double reflectivity;
	//double refractiveIndex;
public:
	RTMaterial() : color(DEFAULT_RTMAT_COLOR), reflectivity(DEFAULT_RTMAT_REFLECTIVITY) {}
	RTMaterial(ColorRGB col, double rflct) : color(col), reflectivity(rflct) {}
	ColorRGB getColor() { return color; }
	double getReflectivity() { return reflectivity;}
};
