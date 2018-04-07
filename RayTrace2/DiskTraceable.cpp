#include "DiskTraceable.h"
#include "GeometryHelpers.h"
#include <gmtl/intersection.h>
#include <iostream>

RayCollisionResult StandTraceable::tryCollision(const gmtl::Rayd ray)
{
	RayCollisionResult result = RayCollisionResult(false);
	if (MyUVMap == nullptr || MyMaterial == nullptr) { return result; }
	//std::cout << "\nrunning";
	double t;
	if (gmtl::intersect(Plane, ray, t)) {
		gmtl::Point3d collisionPos = ray.getOrigin() + ray.getDir()*t;
		if (distanceBetween(Position, collisionPos) < Radius)
		{
			UVCoord coord = MyUVMap->getUV(collisionPos - Position);
			MatSample matSample = MyMaterial->getSample(coord);
			CollisionPoint collisionPoint = CollisionPoint(collisionPos, Normal, matSample);
			result.setCollisionPoint(collisionPoint);
			result.setCollided(true);
		}
	}

	return result;
}
