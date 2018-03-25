#pragma once
#include <string>
#include <vector>
#include <IL\il.h>
#include <IL\ilu.h>
#include <IL\ilut.h>
#define NUM_CHANNELS 3 //number of color channels (3 here because we assume we are using RGB)
						//TODO: refactor coz relies on SkyMap

class DevILImage
{
private:
	ILuint ImageId;
	int Width;
	int Height;
	std::string FilePath;
	std::vector<unsigned char> ImageData;
public:
	DevILImage(std::string filePath);
	~DevILImage();

	std::vector<unsigned char> * getImageData() { return &ImageData; }
	int getWidth() { return Width; }
	int getHeight() { return Height; }
};

