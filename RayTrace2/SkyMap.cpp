#include "SkyMap.h"
#include <gmtl\Generate.h>
#include <gmtl/intersection.h>
#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif
#include <math.h>
#include <iostream>
#include <climits>
#define COLOR_FULL_SCALE 255.0 // TODO: maybe refactor coz its used in RTODevil which should be decoupled



void SkyMap::setImageData(std::vector<unsigned char> *imageData, double width, double height)
{
	ImageData = imageData;
	ImageDataWidth = width;
	ImageDataHeight = height;
}

//void SkyMap::setLightData(std::vector<unsigned char>* lightData, double width, double height)
//{
//	if ((int)width == (int)ImageDataWidth && (int)height == (int)ImageDataHeight) {
//		LightData = lightData;
//		hasLightMap = true;
//	}
//}

void SkyMap::setLight(double u, double v) //TODO: fix skylight to follow map offset
{
	//if (u >= 0 && u < 1.) TODO: check for negative or out of bound input parameters
	Light = SkyLight(u, v, DEFAULT_SKY_SIZE);
	hasLight = true;
}

SkyMapHit SkyMap::hitSkyMap(gmtl::Rayd ray)
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

		int redi = ImageData[0][index];				// [0] is used to access the vector since imageData is a pointer
		int greeni = ImageData[0][index + 1];
		int bluei = ImageData[0][index + 2];
		double red = (double)redi / COLOR_FULL_SCALE;
		double green = (double)greeni / COLOR_FULL_SCALE;
		double blue = (double)bluei / COLOR_FULL_SCALE;
		ColorRGB materialColor = ColorRGB(red, green, blue);

		if (hasLight) {
			double lightValue = Light.lightAt(ray.getOrigin(), u, v);
			//if (lightValue < 0.) // hitSkyLight specifies val < 0. if the ray did not hit
			//{
			//	// if skyLight is not hit, then ray is pointing downwards
			//	// reflect the ray off an artificial "ground" at the skyMap position and the skyLight's up direction as the normal
			//	gmtl::Vec3d incidenceVec = ray.getDir();
			//	gmtl::Vec3d normalVec = Light.getNormal();
			//
			//	gmtl::Vec3d newRayDir = incidenceVec - 2.*dot(incidenceVec, normalVec) * normalVec;
			//	newRayDir = makeNormal(newRayDir);
			//
			//	gmtl::Rayd newRay = gmtl::Rayd(colPos + newRayDir * EPSILON, newRayDir);
			//	lightValue = Light.lightAt(newRay, u, v);
			//	if (lightValue < 0.) {
			//		return SkyMapHit(); // TODO: signal error in this case
			//	}
			//}
			//if (!(lightValue < 0.5 && lightValue > -0.5)) {
			//	std::cout << "\nlight val[" << colPos[0] << "] [" << colPos[1] << "] [" << colPos[2] << "]: " << lightValue;
			//}

			//int lRedi = LightData[0][index];
			//int lGreeni = LightData[0][index + 1];
			//int lBluei = LightData[0][index + 2];
			//double lRed = 10.*((double)lRedi / COLOR_FULL_SCALE);
			//double lGreen = 10.*((double)lGreeni / COLOR_FULL_SCALE);
			//double lBlue = 10.*((double)lBluei / COLOR_FULL_SCALE);
			////if (lRed > 1.) { lRed = 1.; }
			////if (lGreen > 1.) { lGreen = 1.; }
			////if (lBlue > 1.) { lBlue = 1.; }
			//ColorRGB lightColor = ColorRGB(lRed, lGreen, lBlue); //TODO: change light intensity by incidence angle
			////std::cout << "\nlightVal [" << x << "," << y << "] : " << lightColor.R << '\n';
			return SkyMapHit(materialColor, lightValue);
		}

		return SkyMapHit(materialColor);
		//return ColorRGB(xVal, yVal, 1.);
		//return ColorRGB((normal[0] - 1.)/2., (normal[1] - 1.)/2.,(normal[2] - 1.1)/2.);
	}
	return SkyMapHit();
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
