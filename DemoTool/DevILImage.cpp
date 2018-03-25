#include "DevILImage.h"


DevILImage::DevILImage(std::string filePath)
{
	ImageId = 0;
	ilInit();
	iluInit();
	ilGenImages(1, &ImageId);
	ilBindImage(ImageId);
	ilLoadImage(filePath.c_str()); // TODO: check if load failed
	Width = ilGetInteger(IL_IMAGE_WIDTH);
	Height = ilGetInteger(IL_IMAGE_HEIGHT);
	ImageData = std::vector<unsigned char>(Width*Height*NUM_CHANNELS);
	unsigned char * imageData_c = &ImageData[0];
	ilCopyPixels(0, 0, 0, Width, Height, 1, IL_RGB, IL_UNSIGNED_BYTE, imageData_c);
}

DevILImage::~DevILImage()
{
}
