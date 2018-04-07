#pragma once
#include <gmtl\Point.h>
#include "Texture.h"

/*
	Interface for classes responsible for mapping 3d model coordinates to 2D texture files
*/
class UVMap {
public:
	UVMap() {}
	~UVMap() {}
	virtual UVCoord getUV(gmtl::Point3d position) = 0;
};

