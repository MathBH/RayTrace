#pragma once
#include <gmtl/Vec.h>
#include <gmtl/AxisAngle.h>
#include <gmtl/Matrix.h>
#include <gmtl/Generate.h>

double distanceBetween(gmtl::VecBase<double, 3> v1, gmtl::VecBase<double, 3> v2)
{
	gmtl::Vec3d vDelta = v2 - v1;
	double distance = gmtl::length(vDelta);
	return distance;
}