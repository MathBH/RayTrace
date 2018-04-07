#pragma once
#include "TextureMapping.h"
#include <gmtl/Vec.h>

// UV Map which translates a position by flatenning it along a 2D space

class FlatMapping : public UVMap
{
private:
	gmtl::Vec3d XAxis;
	gmtl::Vec3d YAxis;

	double XLength;
	double YLength;

public:
	FlatMapping(gmtl::Vec3d xAxis, gmtl::Vec3d yAxis, double xLength, double yLength)
		: XAxis(xAxis), YAxis(yAxis), XLength(xLength), YLength(yLength) {}
	~FlatMapping() {}
	UVCoord getUV(gmtl::Point3d position);
};

