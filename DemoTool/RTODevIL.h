#pragma once
#include "RTOutput.h"
#include <IL/il.h>
#include <IL/ilu.h>
#include <IL/ilut.h>

/*
	Ray Trace Output DevIL
	
	A RayTracer Output implementation made to work with the DevIL library
*/

class RTODevIL : RTOutput
{
public:
	RTODevIL();
	~RTODevIL();
};

