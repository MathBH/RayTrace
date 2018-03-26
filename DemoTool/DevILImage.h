#pragma once
#include <string>
#include <vector>
#include <IL\il.h>
#include <IL\ilu.h>
#include <IL\ilut.h>
#define RGB_NUM_CHANNELS 3 //number of color channels (3 here because we assume we are using RGB)
						//TODO: refactor coz relies on SkyMap

class DevILImageRGB
{
private:
	ILuint ImageId;
	int Width;
	int Height;
	std::string FilePath;
	std::vector<unsigned char> ImageData;
public:
	DevILImageRGB(std::string filePath);
	~DevILImageRGB();

	std::vector<unsigned char> * getImageData() { return &ImageData; }
	int getWidth() { return Width; }
	int getHeight() { return Height; }
};

class DevILImageL
{
private:
	ILuint ImageId;
	int Width;
	int Height;
	std::string FilePath;
	std::vector<unsigned int> ImageData;
public:
	DevILImageL(std::string filePath);
	~DevILImageL();

	std::vector<unsigned int> * getImageData() { return &ImageData; }
	int getWidth() { return Width; }
	int getHeight() { return Height; }
};
