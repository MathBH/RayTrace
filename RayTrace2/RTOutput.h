#pragma once
#include "ColorRGB.h"
#include <string>
/*
	Ray Trace Output

	Interface for outputs the RayTracer can send data to.
*/

using namespace std;

class RTOutput {
public:
	
	/*
		Reset output with given dimensions
	*/
	virtual void reset(int width, int height) = 0;

	/*
		Set pixel color at x , y
	*/
	virtual int setPixel(int x, int y, ColorRGB color) = 0;
	
	/*
		Write data to filePath
	*/
	virtual int writeToFile(string filePath) = 0;
};