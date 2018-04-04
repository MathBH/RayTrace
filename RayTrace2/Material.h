#pragma once
#include "ParameterTypes.h"
#define DEFAULT_RTMAT_COLOR ColorRGB(1.0,1.0,1.0)
#define DEFAULT_RTMAT_ABSORBTION ColorRGB(0.0,0.0,0.0)
#define DEFAULT_RTMAT_REFLECTIVITY 0.5
#define DEFAULT_RTMAT_REFRACTIVE_INDEX 1.52
#define DEFAULT_RTMAT_LIGHT_VALUE 0.
#define DEFAULT_SHININESS 3
/*
Material compatible with ray tracer
*/
class RTMaterial {
private:
	ColorRGB color;
	ColorRGB absorbtion;
	ColorRGB diffuse;
	ColorRGB specular;
	double refractiveIndex;
	double LightValue;
	double reflectivity;
	double shininess;
	//double refractiveIndex;
public:
	RTMaterial() : color(DEFAULT_RTMAT_COLOR), refractiveIndex(DEFAULT_RTMAT_REFRACTIVE_INDEX), absorbtion(DEFAULT_RTMAT_ABSORBTION), LightValue(DEFAULT_RTMAT_LIGHT_VALUE), shininess(DEFAULT_SHININESS) {}
	RTMaterial(ColorRGB col, double refract) : color(col), refractiveIndex(refract), absorbtion(DEFAULT_RTMAT_ABSORBTION), LightValue(DEFAULT_RTMAT_LIGHT_VALUE), shininess(DEFAULT_SHININESS) {}
	RTMaterial(ColorRGB col, ColorRGB abs, double refract) : color(col), refractiveIndex(refract), absorbtion(abs), LightValue(DEFAULT_RTMAT_LIGHT_VALUE), shininess(DEFAULT_SHININESS) {}
	RTMaterial(ColorRGB col, ColorRGB abs, double refract, double reflect) : color(col), refractiveIndex(refract), absorbtion(abs), reflectivity(reflect), shininess(DEFAULT_SHININESS) {}
	//RTMaterial(ColorRGB col, ColorRGB abs, double refract, double lightValue) : color(col), refractiveIndex(refract), absorbtion(abs), LightValue(lightValue) {}

	void setColor(ColorRGB col) { color = col; }
	ColorRGB getColor() { return color; }

	void setRefractiveIndex(double ref) { refractiveIndex = ref; }
	double getRefractiveIndex() { return refractiveIndex; }

	void setAbsorbtion(ColorRGB abs) { absorbtion = abs; }
	ColorRGB getAbsorbtion() { return absorbtion; }

	void setDiffuse(ColorRGB dif) { diffuse = dif; }
	ColorRGB getDiffuse() { return diffuse; }

	void setSpecular(ColorRGB spec) { specular = spec; }
	ColorRGB getSpecular() { return specular; }

	void setReflectivity(double ref) { reflectivity = ref; }
	double getReflectivity() { return reflectivity; }

	void setShininess(double shiny) { shininess = shiny; }
	double getShininess() { return shininess; }
};