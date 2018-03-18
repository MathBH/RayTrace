#pragma once
#include <gmtl\Point.h>
#include <gmtl\Sphere.h>
#include "TraceableObject.h"
#ifndef	DEFAULT_SPHERE_POS
#ifdef DEFAULT_POS
#define DEFAULT_SPHERE_POS DEFAULT_POS
#else
#define DEFAULT_SPHERE_POS gmtl::Point3d(0.0,0.0,0.0)
#endif
#endif
#ifndef DEFAULT_SPHERE_RADIUS
#define DEFAULT_SPHERE_RADIUS 1.0
#endif

class SphereTraceable : public TraceableObject
{
protected:
	gmtl::Sphered sphereData;

public:
	SphereTraceable() : SphereTraceable(DEFAULT_SPHERE_POS, DEFAULT_SPHERE_RADIUS) {}
	SphereTraceable(gmtl::Point3d p, double r) : sphereData(gmtl::Sphered(p, r)) {}
	~SphereTraceable() {}

	double getRadius() { return sphereData.getRadius(); }
	gmtl::Point3d getPosition() { return sphereData.getCenter(); }

	void setRadius(double r) { sphereData.setRadius(r); }
	void setPosition(gmtl::Point3d p) { sphereData.setCenter(p); }

	RayCollisionResult tryCollision(gmtl::Rayd ray) override;
};