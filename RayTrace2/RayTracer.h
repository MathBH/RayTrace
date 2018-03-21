#pragma once
#include "Scene.h"
#include "RTOutput.h"

/*
	Ray Tracer

	Ray Tracing class

	TODO: add methods to set stuff
*/

class RayTracer
{
public:
	RayTracer();
	~RayTracer();

	int render(Scene& scene, RTOutput& renderOut, int width, int height);
};

