#pragma once
#include <Texture.h>
#include "DevILImage.h"
#define BPP_RGB 3 // Bits per pixel RGB
#define BPP BPP_RGB // Bits per pixel (currently set to rgb)		//TODO: refactor duplicate in RTODevIL potentially make a file with RGB constants
#define RGBCOLOR_FULL_SCALE 255. // TODO: maybe refactor coz its used in RTODevil which should be decoupled

class DevILTexture : public Texture
{
private:
	DevILImageRGB * Source;
	std::vector<unsigned char> * TextureData;
	double Width;
	double Height;

public:
	DevILTexture();
	~DevILTexture();

	void setSource(DevILImageRGB* source);
	ColorRGB getValueAt(UVCoord coord) override;
};

