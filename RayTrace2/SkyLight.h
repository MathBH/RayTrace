#pragma once
#include <gmtl/Ray.h>
#include <gmtl/Plane.h>
#include "ParameterTypes.h"

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

	double intensity;

public:
	SkyLight(double u, double v, double distance);
	SkyLight();
	~SkyLight();

	double lightAt(gmtl::Point3d origin, double u, double v);
	gmtl::Vec3d getNormal() { return LightPlane.getNormal(); }
};

