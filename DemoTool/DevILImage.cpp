#include "DevILImage.h"


DevILImageRGB::DevILImageRGB(std::string filePath)
{
	ImageId = 0;
	ilInit();
	iluInit();
	ilGenImages(1, &ImageId);
	ilBindImage(ImageId);
	ilLoadImage(filePath.c_str()); // TODO: check if load failed
	Width = ilGetInteger(IL_IMAGE_WIDTH);
	Height = ilGetInteger(IL_IMAGE_HEIGHT);
	ImageData = std::vector<unsigned char>(Width*Height*RGB_NUM_CHANNELS);
	unsigned char * imageData_c = &ImageData[0];
	ilCopyPixels(0, 0, 0, Width, Height, 1, IL_RGB, IL_UNSIGNED_BYTE, imageData_c);
	ilBindImage(0);
	ilDeleteImage(ImageId);
}

DevILImageRGB::~DevILImageRGB()
{
}

DevILImageL::DevILImageL(std::string filePath)
{
	ImageId = 0;
	ilInit();
	iluInit();
	ilGenImages(1, &ImageId);
	ilBindImage(ImageId);
	ILboolean resultLoad = ilLoadImage(filePath.c_str()); // TODO: check if load failed
	if (resultLoad == IL_FALSE) {
		exit(-1);
	}
	Width = ilGetInteger(IL_IMAGE_WIDTH);
	Height = ilGetInteger(IL_IMAGE_HEIGHT);
	ImageData = std::vector<unsigned int>(Width*Height);
	unsigned int * imageData_c = &ImageData[0];
	ILboolean resultCopy = ilCopyPixels(0, 0, 0, Width, Height, 1, IL_LUMINANCE, IL_UNSIGNED_INT, imageData_c);
	ilBindImage(0);
	ilDeleteImage(ImageId);
}

DevILImageL::~DevILImageL()
{
}
