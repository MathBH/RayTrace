#include "RayTracer.h"
#include "RayCam.h"
#include <vector>
#include <cmath>
#include <stdio.h>
//#include <boost/math/distributions/normal.hpp>

using namespace gmtl;
/*
Set target output
return:
	0 on success
	negative values for invalid output
*/
int RayTracer::setOutput(RTOutput * rndrOut)
{
	renderOutput = rndrOut;
	outputSet = true; //TODO: check for validity
	return 0;
}

int RayTracer::setScene(RTScene * scn)
{
	scene = scn;
	sceneSet = true; //TODO: check for validity
	return 0;
}

//Helper - TODO: refactor
void RayTracer::insertBufferLine(vector<ColorRGB> pixelBuffer, int yIndex) {
	for (int xIndex = 0; xIndex < renderSettings.resolution.width; xIndex++) {
		renderOutput->setValueAt(xIndex, yIndex, pixelBuffer[xIndex]);
	}
}

Rayd reflect(Rayd ray, CollisionPoint colPoint) {
	Vec3d incidenceVec = ray.getDir();
	Vec3d normalVec = colPoint.getNormal();

	Vec3d newRayDir = incidenceVec - 2.*dot(incidenceVec, normalVec) * normalVec;
	newRayDir = makeNormal(newRayDir);

	Rayd newRay = Rayd(colPoint.getPosition(), newRayDir);
	return newRay;
}

//Debug purpose only value
#define DOF 16.
#define RAY_LIFE 12
ColorRGB RayTracer::trace(RTScene * scene, Rayd ray, int life)
			//TODO: add parameter int life -	also first try collision with lights and then compare if closest object is closer than closest light
			//									if it hits a light, return the light's diffuse color
			//
			//		get material value for reflectivity
			//		get lights in line of sight
			//		calculate val = dot(normalize(lightPos - colPos), f_normal)*matColor*(1-reflectivity)
			//
			//		if life == 0
			//			return val
			//
			//		get reflectedray
			//		I - 2 * dotProduct(I, N) * N;
			//
			//		return val + reflectivity*trace(reflectedray, scene, life-1)
			//
			//		shading(light)
			//			take angle between normal and incidence of light
			//			use that thing from class and yeah return modified material color

			//		refraction:
			//
			//		get refraction index for material
			//		calculate the new refractedray
			//
			//		get transparency
			//		multiply the last value^ by (1-transparency)
			//		return that^ + transparency*trace(refractedray,scene)
			//
			//
			//		refractedray
			//		todo:
			//
			//
			//
			//		
			//		Beer’s Law https://blog.demofox.org/2017/01/09/raytracing-reflection-refraction-fresnel-total-internal-reflection-and-beers-law/
			//		
			//		vec3 color = RayTrace(rayPos, rayDir);
			//		vec3 absorb = exp(-OBJECT_ABSORB * absorbDistance);
			//		color *= absorb;
			//		OBJECT_ABSORB is an RGB value that describes how much of each color channel absorbs over distance.For example, a value of(8.0, 2.0, 0.1) would make red get absorbed the fastest, then green, then blue, so would result in a blueish, slightly green color object.
{
	if (life <= 0) {
		return scene->Sky.hitSkyMap(ray);
	}

	RayCollisionResult collisionResult = scene->tryCollision(ray);
	if (collisionResult.getCollided())
	{
		CollisionPoint collisionPoint = collisionResult.getCollisionPoint();
		//Vec3d colToRay = collisionPoint.getPosition() - ray.getOrigin();
		//double distanceToPoint = gmtl::length(colToRay);
		//double colorValue = 1. - (distanceToPoint / DOF);
		Rayd reflectedRay = reflect(ray, collisionPoint);
		//ColorRGB colorOut = collisionPoint.getMaterial().getColor() * colorValue/2.;
		//colorOut += trace(scene, reflectedRay, life - 1)/2.;
		return trace(scene, reflectedRay, life - 1);
	}
	else {
		return scene->Sky.hitSkyMap(ray);
	}
}

int RayTracer::render() //TODO add filepath to render to
{

	if (!outputSet) {
		return -1;
	}
	if (!sceneSet) {
		return -2;
	}
	const int outputWidth = renderSettings.resolution.width;
	const int outputHeight = renderSettings.resolution.height;

	RayCam rayCam = RayCam(scene->camera, renderSettings.camSettings.zNear, renderSettings.camSettings.fov,
		renderSettings.antiAlias, renderSettings.resolution);

	RayIterator rayIterator = rayCam.iterator();
	renderOutput->initialize(outputWidth, outputHeight);

	std::cout << "\nRENDERING\n---------";
	for (int yIndex = 0; yIndex < outputHeight; yIndex++)
	{
		for (int xIndex = 0; xIndex < outputWidth; xIndex++)
		{
			//std::cout << "\nposition: [" << xIndex << " , " << yIndex << "]";
			RayPacket pixelRays = rayIterator.getAt(xIndex, yIndex);
			ColorRGB colorOut = ColorRGB(0.0,0.0,0.0);
			for (Rayd ray : pixelRays) {
				colorOut += trace(scene, ray, RAY_LIFE);
			}
			colorOut = colorOut/(pow((double)renderSettings.antiAlias, 2.));
			//std::cout << "\n[" << colorOut.R << ", " << colorOut.G << ", " << colorOut.B << "]";
			renderOutput->setValueAt(xIndex, yIndex, colorOut);
		}
		renderOutput->commit();
		std::cout << "\nprogress: " << (yIndex/(double)outputHeight)*100. << "%";
	}
	return 0;
}