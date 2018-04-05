#pragma once
#include <Texture.h>
#include "DevILImage.h"
#define BPP_RGB 3 // Bits per pixel RGB
#define RGBCOLOR_FULL_SCALE ((unsigned char) 0xFF)
#define BPP BPP_RGB // Bits per pixel (currently set to rgb)		//TODO: refactor duplicate in RTODevIL potentially make a file with RGB constants

class DevILTexture : Texture
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
	ColorRGB getValueAt(int x, int y) override;
};

