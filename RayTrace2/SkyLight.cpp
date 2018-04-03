#include "SkyLight.h"
#include <gmtl/Generate.h>
#include <iostream>
#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif
#include <math.h>
#include <gmtl/intersection.h>
#include "Constants.h"
#define DEFAULT_SUN_DISTANCE 9999.
#define DEFAULT_SUN_INTENSITY 10.


SkyLight::SkyLight() : SunDistance(DEFAULT_SUN_DISTANCE), intensity(DEFAULT_SUN_INTENSITY)
{
}


SkyLight::SkyLight(double u, double v, double distance) : SunDistance(DEFAULT_SUN_DISTANCE), intensity(DEFAULT_SUN_INTENSITY)
{
	double y = v*2. - 1.;
	double horizontalRadius = (sqrt(1. - pow(y, 2.)));

	double azimut = u*2*M_PI;
	double x = cos(azimut)*horizontalRadius;
	double z = sin(azimut)*horizontalRadius;
	gmtl::Vec3d normal = gmtl::Point3d(x, y, z); // We use position to make the normal since this is a sphere
	normal = gmtl::makeNormal(normal);
	Distance = distance;
	LightPlane = gmtl::Planed(normal, Distance);
	std::cout << "\nLight Normal: <" << LightPlane.getNormal()[0] << " , " << LightPlane.getNormal()[1] << " , " << LightPlane.getNormal()[2] << ">";
	std::cout << "\nLight Offset: " << LightPlane.getOffset();
}

SkyLight::~SkyLight()
{
}

//double lightRecurse(gmtl::Rayd ray, gmtl::Planed lightPlane, gmtl::Vec3d sunPos) {
//	double t;
//	if (gmtl::intersect<double>(lightPlane, ray, t)) {
//		gmtl::Point3d rayColPos = ray.getOrigin() + ray.getDir() * t;
//		//std::cout << "\nLight Normal: <" << LightPlane.getNormal()[0] << " , " << LightPlane.getNormal()[1] << " , " << LightPlane.getNormal()[2] << ">";
//		//std::cout << "\nSky: " << Distance << " Sun: " << SunDistance;
//		//std::cout << "\nSunPos: <" << sunPos[0] << " , " << sunPos[1] << " , " << sunPos[2] << ">";
//		gmtl::Vec3d sunRay = rayColPos - sunRay;
//
//		sunRay = gmtl::makeNormal(sunRay);
//		gmtl::Vec3d rayNormal = gmtl::makeNormal(ray.getDir());
//
//		double lightTransmitted = gmtl::dot(sunRay, -rayNormal);
//		lightTransmitted = lightTransmitted * 0.5 + 0.5;
//		return lightTransmitted; // result is between 0. and 1.
//	}
//	else
//	{
//		// if skyLight is not hit, then ray is pointing away from it
//		// reflect the ray off sky map sphere
//		gmtl::Vec3d surfaceNormal = -skyPosNormal;
//
//		gmtl::Vec3d newRayDir = rayDir - 2.*dot(rayDir, surfaceNormal) * surfaceNormal;
//		newRayDir = makeNormal(newRayDir);
//
//		gmtl::Rayd newRay = gmtl::Rayd(mapColPosition + newRayDir * EPSILON, newRayDir);
//	}
//}

/*
	Calculates the collision of a ray against the sky light
	return: value between 0. and 1.
			value < 0 if failed
*/
double SkyLight::lightAt(gmtl::Point3d origin, double u, double v)
{
	// if ray from origin to uv pos hits the light plane, return the data
	// else, reflect that ray against an artificial ground derived from up normal

	//We use u,v to decouple the process of getting the light on the sky map from its actual world coordinates
	// which could be altered by the map offsets.
	// This is preferable as it allows artists to position the sun by direct reference to the skymap texture coordinates

	//Convert u, v to a position on the sky map
	double y = v * 2. - 1.;
	double horizontalRadius = (sqrt(1. - pow(y, 2.)));

	double azimut = u * 2 * M_PI;
	double x = cos(azimut)*horizontalRadius;
	double z = sin(azimut)*horizontalRadius;
	gmtl::Vec3d skyPosNormal = gmtl::Point3d(x, y, z); // We use position to make the normal since this is a sphere
	skyPosNormal = gmtl::makeNormal(skyPosNormal); // make sure it is normal
	gmtl::Point3d mapColPosition = skyPosNormal * Distance; // extend the vector to a point on the sky

	gmtl::Vec3d rayDir = mapColPosition - origin; // determine in what direction the ray is going in
	rayDir = gmtl::makeNormal(rayDir);
	gmtl::Rayd ray = gmtl::Rayd(origin, rayDir);

	double t;
	if (gmtl::intersect<double>(LightPlane, ray, t)) {
		gmtl::Point3d rayColPos = ray.getOrigin() + ray.getDir() * t;
		gmtl::Vec3d sunPos = LightPlane.getNormal() * (Distance + SunDistance);
		//std::cout << "\nLight Normal: <" << LightPlane.getNormal()[0] << " , " << LightPlane.getNormal()[1] << " , " << LightPlane.getNormal()[2] << ">";
		//std::cout << "\nSky: " << Distance << " Sun: " << SunDistance;
		//std::cout << "\nSunPos: <" << sunPos[0] << " , " << sunPos[1] << " , " << sunPos[2] << ">";
		gmtl::Vec3d sunRay = rayColPos - sunRay;
	
		sunRay = gmtl::makeNormal(sunRay);
		gmtl::Vec3d rayNormal = gmtl::makeNormal(ray.getDir());
	
		double lightTransmitted = gmtl::dot(sunRay, -rayNormal);
		lightTransmitted = lightTransmitted * 0.5 + 0.5;
		return lightTransmitted * intensity; // result is between 0. and 1.
	}
	else
	{
		//// if skyLight is not hit, then ray is pointing away from it
		//// reflect the ray off sky map sphere
		//gmtl::Vec3d surfaceNormal = -skyPosNormal;
		//
		//gmtl::Vec3d newRayDir = rayDir - 2.*dot(rayDir, surfaceNormal) * surfaceNormal;
		//newRayDir = makeNormal(newRayDir);
		//
		//gmtl::Rayd newRay = gmtl::Rayd(mapColPosition + newRayDir * EPSILON, newRayDir);
		//double t2;
		//if (gmtl::intersect<double>(LightPlane, newRay, t2)) {	//TODO: refactor duplicate
		//	gmtl::Point3d rayColPos = newRay.getOrigin() + newRay.getDir() * t;
		//	gmtl::Vec3d sunPos = LightPlane.getNormal() * (Distance + SunDistance);
		//	//std::cout << "\nLight Normal: <" << LightPlane.getNormal()[0] << " , " << LightPlane.getNormal()[1] << " , " << LightPlane.getNormal()[2] << ">";
		//	//std::cout << "\nSky: " << Distance << " Sun: " << SunDistance;
		//	//std::cout << "\nSunPos: <" << sunPos[0] << " , " << sunPos[1] << " , " << sunPos[2] << ">";
		//	gmtl::Vec3d sunRay = rayColPos - sunRay;
		//
		//	sunRay = gmtl::makeNormal(sunRay);
		//	gmtl::Vec3d rayNormal = gmtl::makeNormal(newRay.getDir());
		//
		//	double lightTransmitted = gmtl::dot(sunRay, -rayNormal);
		//	lightTransmitted = lightTransmitted * 0.5 + 0.5;
		//	//std::cout << "\nlight" << lightTransmitted * intensity;
		//	return lightTransmitted*intensity; // result is between 0. and 1.
		//}
		//else
		//{
			gmtl::Vec3d sunPos = LightPlane.getNormal() * (Distance + SunDistance);
			gmtl::Vec3d sunRay = mapColPosition - sunRay;
			sunRay = gmtl::makeNormal(sunRay);
			double lightTransmitted = gmtl::dot(sunRay, -rayDir);
			return lightTransmitted * intensity; // result is between 0. and 1.
		
		//}
		//lightValue = Light.lightAt(newRay, u, v);
		//if (lightValue < 0.) {
		//	return SkyMapHit(); // TODO: signal error in this case
		//}
	}
	return -1.0; // TODO: mark error
}
