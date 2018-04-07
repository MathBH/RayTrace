#include "FlatMapping.h"
#include <gmtl/Generate.h>

UVCoord FlatMapping::getUV(gmtl::Point3d position)
{
	double u = 0.5 + gmtl::dot(position, XAxis)/XLength*0.5;
	double v = 0.5 + gmtl::dot(position, YAxis)/YLength*0.5;

	return UVCoord(u, v);
}
