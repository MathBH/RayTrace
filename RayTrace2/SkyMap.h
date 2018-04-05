#pragma once
#include "ParameterTypes.h"
#include <vector>
#include <string>
#include <gmtl/Vec.h>
#include <gmtl/Point.h>
#include <gmtl/Ray.h>
#include <gmtl/Sphere.h>
#include <gmtl/AxisAngle.h>
#include <gmtl/Matrix.h>
#include <gmtl/Generate.h>
#include "TraceableObject.h"
#define DEFAULT_SKY_MAP_UP gmtl::Vec3d(0.,1.,0.)
#define DEFAULT_SKY_MAP_FORWARD gmtl::Vec3d(0.,0.,-1.)
#define DEFAULT_SKY_MAP_RIGHT gmtl::Vec3d(1.,0.,0.)
#define NUM_CHANNELS 3 //number of color channels (3 here because we assume we are using RGB) //TODO: refactor coz relies on DevILImage
#define DEFAULT_SKY_SIZE 9999.
#define DEFAULT_SKY_POS gmtl::Point3d (0.,0.,0.)

class SkyMapHit
{
private:
	ColorRGB ColorData;
	ColorRGB LightData;

public:
	SkyMapHit() {}
	SkyMapHit(ColorRGB color) : ColorData(color), LightData(ColorRGB(1.,1.,1.)) {}
	SkyMapHit(ColorRGB color, ColorRGB light) : ColorData(color), LightData(light) {}
	~SkyMapHit() {}

	ColorRGB getColorData() { return ColorData; }
	ColorRGB getLightData() { return LightData; }
};

class SkyMap
{
private:
	int NumChannels;
	gmtl::Sphered collisionSphere;
	std::vector<unsigned char> *ImageData;
	std::vector<unsigned char> *LightData;
	bool hasLightMap;
	double ImageDataWidth;
	double ImageDataHeight;
	gmtl::AxisAngled AzimutOffset;
	gmtl::AxisAngled ElevationOffset;

	gmtl::Matrix44d OffsetAzimCache;
	gmtl::Matrix44d OffsetElevCache;
public:
	SkyMap() : NumChannels(NUM_CHANNELS), collisionSphere(gmtl::Sphered(DEFAULT_SKY_POS,DEFAULT_SKY_SIZE)),
		AzimutOffset(gmtl::AxisAngled(0.,DEFAULT_SKY_MAP_UP)), ElevationOffset(0.,DEFAULT_SKY_MAP_RIGHT), hasLightMap(false) {
		OffsetAzimCache = gmtl::Matrix44d();
		gmtl::setRot(OffsetAzimCache, AzimutOffset);
		OffsetElevCache = gmtl::Matrix44d();
		gmtl::setRot(OffsetElevCache, ElevationOffset);
	}
	~SkyMap() {}

	void setImageData(std::vector<unsigned char> *imageData, double width, double height);
	void setLightData(std::vector<unsigned char> *lightData, double width, double height);
	void setAzimutOffset(double azimut) { AzimutOffset.setAngle(azimut); gmtl::setRot(OffsetAzimCache, AzimutOffset); }
	void setElevationOffset(double azimut) { ElevationOffset.setAngle(azimut); gmtl::setRot(OffsetElevCache, ElevationOffset); }

	RayCollisionResult hitSkyMap(gmtl::Rayd ray);
};

