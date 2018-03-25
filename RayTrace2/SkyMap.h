#pragma once
#include "ParameterTypes.h"
#include <vector>
#include <string>
#include <gmtl/Vec.h>
#include <gmtl/Point.h>
#include <gmtl/Ray.h>
#include <gmtl/Sphere.h>
#define DEFAULT_SKY_MAP_UP gmtl::Vec3d(0.,1.,0.)
#define DEFAULT_SKY_MAP_FORWARD gmtl::Vec3d(0.,0.,-1.)
#define DEFAULT_SKY_MAP_RIGHT gmtl::Vec3d(1.,0.,0.)
#define NUM_CHANNELS 3 //number of color channels (3 here because we assume we are using RGB) //TODO: refactor coz relies on DevILImage
#define DEFAULT_SKY_SIZE 9999.
#define DEFAULT_SKY_POS gmtl::Point3d (0.,0.,0.)

class SkyMap
{
private:
	int NumChannels;
	gmtl::Sphered collisionSphere;
	std::vector<unsigned char> *ImageData;
	double ImageDataWidth;
	double ImageDataHeight;
	int OffsetU;
	int OffsetV;
public:
	SkyMap() : NumChannels(NUM_CHANNELS), collisionSphere(gmtl::Sphered(DEFAULT_SKY_POS,DEFAULT_SKY_SIZE)),
		OffsetU(0), OffsetV(0) {}
	~SkyMap() {}

	void setImageData(std::vector<unsigned char> *imageData, double width, double height);

	ColorRGB hitSkyMap(gmtl::Rayd ray);
};

