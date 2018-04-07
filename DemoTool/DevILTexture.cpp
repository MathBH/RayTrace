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
	//std::cout << "\nW: " << Width << "\nH" << Height;
}

ColorRGB DevILTexture::getValueAt(UVCoord coord)
{
	//std::cout << "\ncoord: " << coord.U << " " << coord.V;
	//return ColorRGB(coord.U, coord.V, 1.);

	if (Source == nullptr)
	{
		std::cerr << "\nDevILTexture ERROR: Could not getValueAt(), source is a null pointer";
		return ColorRGB();
	}
	int x = (int)round(coord.U*Width) % (int)Width;
	int y = (int)round(coord.V*Height) % (int)Height;
	int i = y * Width*BPP + x * BPP;
	//std::cout << "\nindex : " << i;
	//std::cout << "coord: " << coord.U << " " << coord.V;
	int redi = TextureData[0][i];
	int greeni = TextureData[0][i + 1];
	int bluei = TextureData[0][i + 2];
	double red = (double)redi / RGBCOLOR_FULL_SCALE;
	double green = (double)greeni / RGBCOLOR_FULL_SCALE;
	double blue = (double)bluei / RGBCOLOR_FULL_SCALE;
	ColorRGB materialColor = ColorRGB(red, green, blue);
	//std::cout << "\nCOL: " << redi << " " << greeni << " " << bluei << "/" << RGBCOLOR_FULL_SCALE;
	
	return materialColor;
}
