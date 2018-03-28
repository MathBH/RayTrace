#pragma once
#include "ParameterTypes.h"
#define DEFAULT_RTMAT_COLOR ColorRGB(1.0,1.0,1.0)
#define DEFAULT_RTMAT_REFLECTIVITY 0.5
#define DEFAULT_RTMAT_REFRACTIVE_INDEX 1.52
/*
	Material compatible with ray tracer
*/
class RTMaterial {
private:
	ColorRGB color;
	double refractiveIndex;
	//double refractiveIndex;
public:
	RTMaterial() : color(DEFAULT_RTMAT_COLOR), refractiveIndex(DEFAULT_RTMAT_REFRACTIVE_INDEX) {}
	RTMaterial(ColorRGB col, double refract) : color(col), refractiveIndex(refract) {}
	ColorRGB getColor() { return color; }
	double getRefractiveIndex() { return refractiveIndex;}
};
