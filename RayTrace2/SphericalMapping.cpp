#include "SphericalMapping.h"

void SphericalMapping::setTexture(Texture& tex)
{
	texture = tex;
}

ColorRGB SphericalMapping::getValueAt(gmtl::Point3d surfacePos, gmtl::Point3d center)
{
	return ColorRGB();
}
