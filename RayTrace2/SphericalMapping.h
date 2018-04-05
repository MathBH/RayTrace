#pragma once
#include "TextureMapping.h"
#include "ParameterTypes.h"
#include <gmtl\Sphere.h>
#include <gmtl\Point.h>

class SphericalMapping : public TextureMapping {
private:
	Texture & texture;
public:
	void setTexture(Texture& tex) override;
	ColorRGB getValueAt(gmtl::Point3d surfacePos, gmtl::Point3d center);
};