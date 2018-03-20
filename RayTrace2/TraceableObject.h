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
private:
	gmtl::Vec3d normal;
	gmtl::Point3d position;

public:

	CollisionPoint() : normal(DEFAULT_POINT_NORM), position(DEFAULT_POINT_POS){}
	CollisionPoint(gmtl::Point3d p, gmtl::Vec3d n) : normal(n), position(p){}

	gmtl::Vec3d getNormal() {
		return gmtl::Vec3d(normal);
	}

	gmtl::Point3d getPosition() {
		return gmtl::Point3d(position);
	}
};

class RayCollisionResult {
private:
	bool collided;
	CollisionPoint collisionPoint;
public:

	RayCollisionResult() : collided(false), collisionPoint(CollisionPoint()){}
	RayCollisionResult(bool col, const CollisionPoint& colPoint) : collided(col), collisionPoint(colPoint){}

	void setCollided(bool col)
	{
		collided = col;
	}
	void setCollisionPoint(const CollisionPoint& colPoint) {
		collisionPoint = colPoint;
	}
	bool getCollided()
	{
		return collided;
	}
	CollisionPoint getCollisionPoint() {
		return collisionPoint;
	}
};

class TraceableObject {
public:
	virtual RayCollisionResult tryCollision(const gmtl::Rayd ray) = 0;
};