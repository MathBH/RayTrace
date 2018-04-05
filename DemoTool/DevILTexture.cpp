#include "DevILTexture.h"
#include <iostream>

DevILTexture::DevILTexture()
{
}


DevILTexture::~DevILTexture()
{
}

void DevILTexture::setSource(DevILImageRGB * source)
{
	if (source == nullptr)
	{
		std::cerr << "\nDevILTexture ERROR: Could not setSource(), source is a null pointer";
		return;
	}
	Source = source;
	TextureData = Source->getImageData();
	Width = Source->getWidth();
	Height = Source->getHeight();
}

ColorRGB DevILTexture::getValueAt(int x, int y)
{
	if (Source == nullptr)
	{
		std::cerr << "\nDevILTexture ERROR: Could not getValueAt(), source is a null pointer";
		return ColorRGB();
	}

	int i = y * Width*BPP + x * BPP;
	int redi = TextureData[0][i];
	int greeni = TextureData[0][i + 1];
	int bluei = TextureData[0][i + 2];
	double red = (double)redi / RGBCOLOR_FULL_SCALE;
	double green = (double)greeni / RGBCOLOR_FULL_SCALE;
	double blue = (double)bluei / RGBCOLOR_FULL_SCALE;
	ColorRGB materialColor = ColorRGB(red, green, blue);

	return materialColor;
}
