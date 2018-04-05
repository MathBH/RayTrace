#include "RTODevIL.h"
#include <iostream>
#define COLOR_FULL_SCALE ((unsigned char) 0xFF)
#define RED_LUMINANCE 0.299
#define GREEN_LUMINANCE 0.587
#define BLUE_LUMINANCE 0.114


/*
Calculate the weighted luminance for a given color
*/
double luminance(ColorRGB color) {
	return color.R*RED_LUMINANCE + color.G*GREEN_LUMINANCE + color.B*BLUE_LUMINANCE;
}

/*
	Initialize output of specified width and height
	return:	 0 on success
			-1 on width or height <= 0
*/
int RTODevIL::initialize(int width, int height)
{
	Width = width;
	Height = height;
	if (!(width > 0 && height > 0))
	{
		return -1;
	}
	imageData = vector<unsigned char>(width*height*BPP);
	ilInit();
	iluInit();
	ImageId = 0;
	ilGenImages(1, &ImageId);
	ilBindImage(ImageId);
	initialized = true;
	return 0;
}

/*
	Set output image file path
	return:	 0 on success
			-1 on invalid path
*/
int RTODevIL::setFilePath(string path)
{
	filePath = path;
	// TODO: check that the file path is valid
	filePathValid = true;
	return 0;
}

/*
	Set the color of the pixel at (x,y) to ColorRGB color

	return:  0 on success
			-1 if not initialized
			-2 if x,y is out of bounds
*/
int RTODevIL::setValueAt(int x, int y, ColorRGB color)
{
	if (!initialized) {
		return -1;

	}
	if (x < Width && y < Height) {
		int i = y * Width*BPP + x*BPP;

		//double colMax = 1.;
		//double lum = luminance(color);
		//double sum = color.R + color.G + color.B;
		double r = color.R;
		double g = color.G;
		double b = color.B;
		//if (lum > 1.) {
		//	r = r / lum;
		//	g = g / lum;
		//	b = b / lum;
		//}
		if (r > 1.) { r = 1.; }
		if (g > 1.) { g = 1.; }
		if (b > 1.) { b = 1.; }
		if (r < 0.) { r = 0.; }
		if (g < 0.) { g = 0.; }
		if (b < 0.) { b = 0.; }

		imageData[i] = COLOR_FULL_SCALE * r;
		imageData[i+1] = COLOR_FULL_SCALE * g;
		imageData[i+2] = COLOR_FULL_SCALE * b;
		return 0;
	}
	return -2;
}

/*
	Write the data in the buffer to filePath

	return:
		 0 on success
		-1 if not initialized
		-2 invalid output file path
		-3 if TexImage could not be generated
		-4 if image could not be saved
*/
int RTODevIL::commit()
{
	if (!initialized) {
		return -1;
	}

	if (!filePathValid) {
		return -2;
	}

	unsigned char * imageData_c = &imageData[0];

	if (!ilTexImage(Width, Height, 1, 3, IL_RGB, IL_UNSIGNED_BYTE, imageData_c)) {
		return -3;
	}

	ilEnable(IL_FILE_OVERWRITE);
	if (!ilSave(IL_PNG, filePath.c_str())) {
		return -4;
	}

	GLuint openglID = ilutGLBindTexImage();

	ilBindImage(0);
	//glDeleteTextures(1, &openglID);
	ilDeleteImage(ImageId);
	return 0;
}
