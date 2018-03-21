#pragma once
#include "RTOutput.h"
#include <IL/il.h>
#include <IL/ilu.h>
#include <IL/ilut.h>
#include <vector>
#define DEFAULT_OUTPUT_WIDTH 640
#define DEFAULT_OUTPUT_HEIGHT 480

#define BPP_RGB 3

#define BPP BPP_RGB
/*
	Ray Trace Output DevIL
	
	A RayTracer Output implementation made to work with the DevIL library
*/


using namespace std;

class RTODevIL : RTOutput
{
private:
	int Width;
	int Height;
	ILuint ImageId;
	vector<unsigned char> imageData;

	/*
		Common initialization calls between constructors
	*/
	void init();
public:
	RTODevIL() : Width(DEFAULT_OUTPUT_WIDTH), Height(DEFAULT_OUTPUT_HEIGHT)
	{
		init();
	}
	RTODevIL(int width, int height) : Width(width), Height(height)
	{
		init();
	}
	~RTODevIL() {}

	/*
		Reset output with given dimensions
	*/
	virtual void reset(int width, int height) override;

	/*
		Set the color of the pixel at (x,y) to ColorRGB color
	*/
	int setPixel(int x, int y, ColorRGB color) override;

	/*
		Write the data in the buffer to filePath
	*/
	int writeToFile(string filePath) override;
};

