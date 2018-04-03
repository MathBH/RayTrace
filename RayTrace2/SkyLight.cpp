#include "SkyLight.h"
#include <gmtl/Generate.h>
#include <iostream>
#include <gmtl/intersection.h>
#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif
#include <math.h>


SkyLight::SkyLight()
{
}


SkyLight::SkyLight(double u, double v, double distance) : SunDistance(DEFAULT_SUN_DISTANCE)
{
	double y = v*2. - 1.;
	double horizontalRadius = (sqrt(1. - pow(y, 2.)));

	double azimut = u*2*M_PI;
	double x = cos(azimut)*horizontalRadius;
	double z = sin(azimut)*horizontalRadius;
	gmtl::Vec3d normal = gmtl::Point3d(x, y, z); // We use position to make the normal since this is a sphere
	normal = gmtl::makeNormal(normal);
	LightPlane = gmtl::Planed(normal, distance);
	std::cout << "\nLight Normal: <" << LightPlane.getNormal()[0] << " , " << LightPlane.getNormal()[1] << " , " << LightPlane.getNormal()[2] << ">";
	std::cout << "\nLight Offset: " << LightPlane.getOffset();
}

SkyLight::~SkyLight()
{
}

double SkyLight::hitSkyLight(gmtl::Rayd ray)
{
	double t;
	if (gmtl::intersect<double>(LightPlane, ray, t)) {
		gmtl::Point3d colPos = ray.getOrigin() + ray.getDir() * t;
		gmtl::Vec3d sunPos = LightPlane.getNormal() * (Distance + SunDistance);
		gmtl::Vec3d sunRay = colPos - sunRay;

		sunRay = gmtl::makeNormal(sunRay);
		gmtl::Vec3d rayNormal = gmtl::makeNormal(ray.getDir());

		double lightTransmitted = gmtl::dot(sunRay, rayNormal);
		return lightTransmitted;
	}
	return 0.0;
}
