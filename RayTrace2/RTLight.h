#pragma once
#include <gmtl/Ray.h>
#include <gmtl/Point.h>
#include <gmtl/Plane.h>

class LightCollisionResult {
public:
	bool collided;
	double illumination;

	LightCollisionResult() {}
	~LightCollisionResult() {}
};

class RTLight
{
private:
	double radius;
	double brightness;
	double falloff;
	gmtl::Point3d position;

public:
	RTLight() {}
	RTLight(gmtl::Point3d pos) : position(pos) {}
	~RTLight() {}

	LightCollisionResult tryCollision(const gmtl::Rayd ray);
	gmtl::Point3d getPosition();
	void setRadius(double rad) { radius = rad; }
	void setBrightness(double bright) { brightness = bright; }
	void setPosition(gmtl::Point3d pos) { position = pos; }
};