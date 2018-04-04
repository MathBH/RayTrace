#include "SphereTraceable.h"
#include <gmtl/intersection.h>
#include <gmtl/Generate.h>
#define EPSILON 0.000000001 // custom defined for now for the sake of control 

RayCollisionResult SphereTraceable::tryCollision(gmtl::Rayd ray)
{
	RayCollisionResult result = RayCollisionResult(false);
	int numHits;
	double t0, t1, t;

	if (gmtl::intersect<double>(sphereData, ray, numHits, t0, t1)) {
		t = (numHits > 1 && t1 < t0 && t1 > 0) ? t1 : t0;
		if (t <= 0) { return result; }

		gmtl::Point3d colPos = ray.getOrigin() + ray.getDir() * t;
		gmtl::Vec3d normal = colPos - sphereData.getCenter();
		normal = gmtl::makeNormal(normal);
		CollisionPoint collisionPoint = CollisionPoint(colPos, normal, material);

		result.setCollided(true);
		result.setCollisionPoint(collisionPoint);
	}

	return result;
}