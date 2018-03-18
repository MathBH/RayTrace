#pragma once
#include <gmtl\Vec.h>
#include <gmtl\Point.h>
#include <gmtl\Ray.h>

#ifndef	DEFAULT_POINT_POS
#define DEFAULT_POINT_POS gmtl::Point3d(0.0,0.0,0.0)
#endif
#ifndef	DEFAULT_POINT_NORM
#define DEFAULT_POINT_NORM gmtl::Point3d(0.0,1.0,0.0)
#endif
#ifndef	DEFAULT_REFLECTIVE_INDEX
#define DEFAULT_REFLECTIVE_INDEX 0.5
#endif
#ifndef	DEFAULT_REFRACTIVE_INDEX
#define DEFAULT_REFRACTIVE_INDEX 0.5
#endif

class CollisionPoint
{
public:
	gmtl::Vec3d normal;
	gmtl::Point3d position;
	double reflectiveIndex;
	double refractiveIndex;

	CollisionPoint() : normal(DEFAULT_POINT_NORM), position(DEFAULT_POINT_POS),
		reflectiveIndex(DEFAULT_REFLECTIVE_INDEX), refractiveIndex(DEFAULT_REFRACTIVE_INDEX){}
	CollisionPoint(gmtl::Point3d p, gmtl::Vec3d n, double rflct, double rfrct) {
		normal = n;
		position = p;
		reflectiveIndex = rflct;
		refractiveIndex = rfrct;
	}
};

class RayCollisionResult {
public:
	bool collision;
	CollisionPoint collisionPoint;

	RayCollisionResult() : collision(false), collisionPoint(CollisionPoint()){}
	RayCollisionResult(bool col, CollisionPoint colPoint) : collision(col), collisionPoint(colPoint){}
};

class TraceableObject {
public:
	virtual RayCollisionResult tryCollision(gmtl::Rayd ray) = 0;
};