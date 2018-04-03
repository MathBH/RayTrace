#pragma once
#include <gmtl/Ray.h>
#include <gmtl/Plane.h>
#include "ParameterTypes.h"
#define DEFAULT_SUN_DISTANCE 9999.

class SkyLight
{
private:
	//gmtl::Point3d Position;
	//gmtl::Vec3d Normal;
	//ColorRGB Color;
	gmtl::Planed LightPlane;

	double U;
	double V;
	double Distance;
	double SunDistance;

public:
	SkyLight(double u, double v, double distance);
	SkyLight();
	~SkyLight();

	double hitSkyLight(gmtl::Rayd ray);
};

