#include "RayTracer.h"
#include "RayCam.h"
#include <vector>
#include <cmath>
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

int RayTracer::setScene(Scene * scn)
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

//Debug purpose only value
#define DOF 16.
ColorRGB RayTracer::trace(Scene * scene, Rayd ray) {
	bool foundCollision = false; //TODO: refactor
	CollisionPoint closestCollision;
	Point3d cameraPosition = scene->camera.position;
	for (TraceableObject * object : scene->objects.traceable)
	{
		RayCollisionResult result = object->tryCollision(ray);
		if (result.getCollided())
		{
			CollisionPoint collisionPoint = result.getCollisionPoint();
			if (!foundCollision)
			{
				closestCollision = collisionPoint;
				foundCollision = true;
			}
			else
			{
				Vec3d colToCamera = collisionPoint.getPosition() - cameraPosition;
				Vec3d closestColToCamera = closestCollision.getPosition() - cameraPosition;
				if (gmtl::length(colToCamera) < gmtl::length(closestColToCamera))
				{
					closestCollision = collisionPoint;
				}
			}
		}
	}

	if (!foundCollision)
	{
		return ColorRGB();
	}
	else
	{
		Vec3d colToCamera = closestCollision.getPosition() - cameraPosition;
		double distanceToPoint = gmtl::length(colToCamera);
		double colorValue = 1. - (distanceToPoint / DOF);
		//std::cout << "\ndistance " << distanceToPoint
		//	<< "\ncolor " << colorValue;
		return ambientColor * colorValue;
	}
	//TODO: implement
	//Vec3d dir = ray.getDir();
	//ColorRGB color(abs(dir[0]), abs(dir[1]), abs(dir[2]));
	//return color;
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
	for (int xIndex = 0; xIndex < outputWidth; xIndex++)
	{
		for (int yIndex = 0; yIndex < outputHeight; yIndex++)
		{
			//std::cout << "\nposition: [" << xIndex << " , " << yIndex << "]";
			RayPacket pixelRays = rayIterator.getAt(xIndex, yIndex);
			ColorRGB colorOut = ColorRGB(0.0,0.0,0.0);
			for (Rayd ray : pixelRays) {
				colorOut += trace(scene, ray);
			}
			colorOut = colorOut/(pow((double)renderSettings.antiAlias, 2.));
			//std::cout << "\n[" << colorOut.R << ", " << colorOut.G << ", " << colorOut.B << "]";
			renderOutput->setValueAt(xIndex, yIndex, colorOut);
		}
		std::cout << "\nprogress: " << (xIndex/(double)outputWidth)*100. << "%";
	}
	return 0;
}
