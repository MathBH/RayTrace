#pragma once
#include "RTOutput.h"
#include <IL/il.h>
#include <IL/ilu.h>
#include <IL/ilut.h>
#include <vector>
#include "DevILGLWindow.h"
#define DEFAULT_OUTPUT_WIDTH 640
#define DEFAULT_OUTPUT_HEIGHT 480

#define BPP_RGB 3

#define BPP BPP_RGB
/*
	Ray Trace Output DevIL
	
	A RayTracer Output implementation made to work with the DevIL library
*/


using namespace std;

class RTODevIL : public RTOutput
{
private:
	int Width;
	int Height;
	string filePath;
	ILuint ImageId;
	bool initialized;
	bool filePathValid;
	DevILGLWindow Window;
	bool windowAssigned;

	vector<unsigned char> imageData;
public:
	RTODevIL() : initialized(false), filePathValid(false), windowAssigned(false){}
	~RTODevIL() {}

	/*
		Set output image file path
		return 0 on success, negative values on failure
	*/
	int setFilePath(string path);
	/*
		Initialize output of specified width and height
		return 0 on success, negative values on failure
	*/
	int initialize(int width, int height) override;

	/*
		Set ColorRGB data at x , y
		return 0 on success, negative values on failure
	*/
	int setValueAt(int x, int y, ColorRGB color) override;

	/*
		Commit data in buffer
		Write the data in the buffer to filePath
		return 0 on success, negative values on failures
	*/
	int commit() override;
};
