#include "SphericalMapping.h"
#include <gmtl\VecOps.h>
#include <gmtl\Generate.h>
#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif
#include <math.h>

UVCoord SphericalMapping::getUV(gmtl::Point3d position)
{
	gmtl::Vec3d normal = gmtl::makeNormal(gmtl::Vec3d(position));

	double u = atan2(normal[0], normal[2]) / (M_PI * 2) + 0.5;
	double v = 0.5 - asin(normal[1])/M_PI;

	return UVCoord(u, v);
}
