#include "RayTracer.h"

RayTracer::RayTracer()
{
}


RayTracer::~RayTracer()
{
}

using namespace gmtl;

int RayTracer::render(Scene& scene, RTOutput& renderOut, int width, int height) //TODO add filepath to render to
{
	scene.camera.setResolution(ResolutionSettings(width, height));
	renderOut.reset(width, height);
	//trace

	return 0;
}

int trace(Scene& scene, Rayd ray) {
	return 0;
}