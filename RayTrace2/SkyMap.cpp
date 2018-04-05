#include "SkyMap.h"
#include <gmtl\Generate.h>
#include <gmtl/intersection.h>
#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif
#include <math.h>
#include <iostream>
#include <climits>
#define RGBCOLOR_FULL_SCALE 255. // TODO: maybe refactor coz its used in RTODevil which should be decoupled
#define VACUUM_REFRACTIVE_INDEX 1.

void SkyMap::setImageData(std::vector<unsigned char> *imageData, double width, double height)
{
	ImageData = imageData;
	ImageDataWidth = width;
	ImageDataHeight = height;
}

void SkyMap::setLightData(std::vector<unsigned char>* lightData, double width, double height)
{
	if ((int)width == (int)ImageDataWidth && (int)height == (int)ImageDataHeight) {
		LightData = lightData;
		hasLightMap = true;
	}
}

RayCollisionResult SkyMap::hitSkyMap(gmtl::Rayd ray)
{
	//TODO: add function to set sky rotation

	int numHits;
	double t0, t1, t;
	if (gmtl::intersect<double>(collisionSphere, ray, numHits, t0, t1)) {
		t = (numHits > 1 && t1 < t0) ? t1 : t0;
		gmtl::Point3d colPos = ray.getOrigin() + ray.getDir() * t;
		gmtl::Vec3d normal = collisionSphere.getCenter() - colPos;
		normal = OffsetElevCache * normal;
		normal = OffsetAzimCache * normal;
		normal = gmtl::makeNormal(normal);

		double u = atan2(normal[0],normal[2]) / (M_PI*2) + 0.5;
		double v = normal[1] * 0.5 + 0.5;

		int x = (int)round(u * ImageDataWidth) % (int)ImageDataWidth;
		int y = (int)round(v * ImageDataHeight) % (int)(ImageDataHeight);

		int index = x * NumChannels + y * ImageDataWidth*NumChannels;

		int redi = ImageData[0][index];
		int greeni = ImageData[0][index + 1];
		int bluei = ImageData[0][index + 2];
		double red = (double)redi / RGBCOLOR_FULL_SCALE;
		double green = (double)greeni / RGBCOLOR_FULL_SCALE;
		double blue = (double)bluei / RGBCOLOR_FULL_SCALE;
		ColorRGB materialColor = ColorRGB(red, green, blue);
		RTMaterial material = RTMaterial(materialColor, VACUUM_REFRACTIVE_INDEX);

		CollisionPoint collisionPoint = CollisionPoint(colPos, normal, material);

		//if (hasLightMap) {
		//	int lRedi = LightData[0][index];
		//	int lGreeni = LightData[0][index + 1];
		//	int lBluei = LightData[0][index + 2];
		//	double lRed = 10.*((double)lRedi / COLOR_FULL_SCALE);
		//	double lGreen = 10.*((double)lGreeni / COLOR_FULL_SCALE);
		//	double lBlue = 10.*((double)lBluei / COLOR_FULL_SCALE);
		//	//if (lRed > 1.) { lRed = 1.; }
		//	//if (lGreen > 1.) { lGreen = 1.; }
		//	//if (lBlue > 1.) { lBlue = 1.; }
		//	ColorRGB lightColor = ColorRGB(lRed, lGreen, lBlue); //TODO: change light intensity by incidence angle
		//	//std::cout << "\nlightVal [" << x << "," << y << "] : " << lightColor.R << '\n';
		//	return SkyMapHit(materialColor,lightColor);
		//}

		return RayCollisionResult(true, collisionPoint);
		//return ColorRGB(xVal, yVal, 1.);
		//return ColorRGB((normal[0] - 1.)/2., (normal[1] - 1.)/2.,(normal[2] - 1.1)/2.);
	}
	return RayCollisionResult(false);
	//RayCollisionResult result = collisionSphere.tryCollision(ray);
	//if (!result.getCollided()) {
	//	return ColorRGB();
	//}
	//CollisionPoint collisionPoint = result.getCollisionPoint();
	//gmtl::Vec3d colNormal = collisionPoint.getNormal(); // get the normal of the sky collision sphere
	//
	//return ColorRGB(colNormal[0], colNormal[1], colNormal[2]);
	//gmtl::Vec3d colNormalXZ = gmtl::Vec3d(colNormal[0], colNormal[1], 0.); //<--- flatten z
	//colNormalXZ = gmtl::makeNormal(colNormalXZ);
	//double thetaPivot = acos(gmtl::dot(colNormalXZ, Forward));  //<--- angle between ray Z flattened and forward vector
	//if (gmtl::dot(colNormalXZ, Right) < 0)
	//{
	//	thetaPivot = (2 * M_PI) - thetaPivot;
	//}
	//
	//double thetaElevation = acos(dot(colNormal, Up)); //<-- angle between ray direction and the Up 
	////std::cout << "\n\npivot: " << thetaPivot;
	////std::cout << "\nelev: " << thetaElevation;
	//
	//double xVal = (thetaPivot / 2 * M_PI);	// <- so that vertical seam is facing backwards
	//double yVal = (thetaElevation / M_PI);	//<- we do this so that y = 0 is when the ray is pointing down
	//
	//int x = xVal* ImageDataWidth;
	//int y = yVal* ImageDataHeight;
	//int index = x * NumChannels + y * ImageDataWidth*NumChannels;
	////std::cout << "\nindex: " << index;
	//int redi = ImageData[0][index]; // [0] is used to access the vector since imageData is a pointer
	//int greeni = ImageData[0][index+1];
	//int bluei = ImageData[0][index+2];
	//
	//double red = (double)redi / COLOR_FULL_SCALE;
	//double green = (double)greeni / COLOR_FULL_SCALE;
	//double blue = (double)bluei / COLOR_FULL_SCALE;
	//
	//return ColorRGB(red,green,blue);
}
