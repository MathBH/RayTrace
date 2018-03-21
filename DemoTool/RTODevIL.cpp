#include "RTODevIL.h"
#define COLOR_FULL_SCALE ((unsigned char) 0xFF)

/*
	Common initialization calls between constructors

	Initialize DevIL and non-primitive class members
*/
void RTODevIL::init()
{
	ilInit();
	iluInit();
	ilutRenderer(ILUT_OPENGL);
	imageData = vector<unsigned char>(Width*Height*BPP);

	for (int i = 0; i < imageData.size(); i += BPP)
	{
		imageData[i] = 255;
		imageData[i+1] = 255;
		imageData[i+2] = 255;
	}
}

/*
	Reset output with given dimensions
*/
void RTODevIL::reset(int width, int height)
{
	Width = width;
	Height = height;
	imageData = vector<unsigned char>(Width*Height*BPP);
	for (int i = 0; i < imageData.size(); i += BPP)
	{
		imageData[i] = 255;
		imageData[i + 1] = 255;
		imageData[i + 2] = 255;
	}
}

/*
	Set the color of the pixel at (x,y) to ColorRGB color

	return: 0 on success, -1 if x,y is out of bounds
*/
int RTODevIL::setPixel(int x, int y, ColorRGB color)
{
	if (x < Width && y < Height) {
		int i = y * Width*BPP + x*BPP;
		imageData[i] = COLOR_FULL_SCALE * color.R;
		imageData[i+1] = COLOR_FULL_SCALE * color.G;
		imageData[i+2] = COLOR_FULL_SCALE * color.B;
		return 0;
	}
	return -1;
}

/*
	Write the data in the buffer to filePath

	return:
		0 on success
		-1 if TexImage could not be generated
		-2 if image could not be saved
*/
int RTODevIL::writeToFile(string filePath)
{
	unsigned char * imageData_c = &imageData[0];

	ImageId = 0;
	ilGenImages(1, &ImageId);
	ilBindImage(ImageId);
	if (!ilTexImage(Width, Height, 1, 3, IL_RGB, IL_UNSIGNED_BYTE, imageData_c)) {
		return -1;
	}
	ilEnable(IL_FILE_OVERWRITE);
	if (!ilSave(IL_PNG, "outputB.png")) {
		return -2;
	}
	ilBindImage(0);
	ilDeleteImage(ImageId);
	return 0;
}
