#pragma once
#include "Texture.h"
/*
	Interface for classes responsible for mapping 3d model coordinates to 2D texture files
*/

class TextureMapping {
public:
	virtual void setTexture(Texture& tex) = 0;
};

