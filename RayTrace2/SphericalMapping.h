#pragma once
#include "TextureMapping.h"
#include "ParameterTypes.h"
#include <gmtl\Sphere.h>
#include <iostream>

/*
	UVMap implementation for spherical objects
*/
class SphericalMapping : public UVMap {
public:
	SphericalMapping() {}
	~SphericalMapping() {}

	/*
		Takes a point relative to the center of a sphere and returns the UV texture coordinates
	*/
	UVCoord getUV(gmtl::Point3d position) override;
};