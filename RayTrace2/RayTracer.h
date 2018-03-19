#pragma once
#include "Scene.h"
#include "RayCam.h"

class RayTracer
{
public:
	Scene scene; //TODO: refactor to set scene where u create a copy image in the ray tracer.
	RayTracer();
	~RayTracer();

	/*
		TODO: find a library and make a method to attatch an output for the raytracer to draw to and make
		a command that calls trace(ray)
			^- idea is just to allocate memory and store image info somewhere <- and return the address
					^- or rather just make the scene output a "colorStream" if that's a thing which u could then
					pass to wtv <- so an output stream? <- ya <- yeah you could probs do something with bytes or wtv
					^- but also if there's a less pretty but quick way to do it go for that first

		write trace(ray) which I expect will return a color value
	*/
};

