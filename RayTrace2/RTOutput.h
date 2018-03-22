#pragma once
#include <string>
#include "ParameterTypes.h"
/*
	Ray Trace Output

	Interface for outputs the RayTracer can send data to.
*/

using namespace std;

class RTOutput {
public:

	/*
		Initialize output of specified width and height
		return 0 on success, negative values on failure
	*/
	virtual int initialize(int width, int height) = 0;
	///*
	//	Get output dimensions
	//*/
	//virtual Dimensions2D getDimensions() = 0;

	/*
		Set ColorRGB data at x , y
		return 0 on success, negative values on failure
	*/
	virtual int setValueAt(int x, int y, ColorRGB color) = 0;
	
	/*
		Commit data in buffer
		return 0 on success, negative values on failures
	*/
	virtual int commit() = 0;
};