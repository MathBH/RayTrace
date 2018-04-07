#pragma once
#include <gmtl\Vec.h>
#include <gmtl\Point.h>
#include <gmtl\Ray.h>
#include "Material.h"
#include "AllTextures.h"

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
	MatSample material;
	gmtl::Vec3d normal;
	gmtl::Point3d position;

public:

	CollisionPoint() : normal(DEFAULT_POINT_NORM), position(DEFAULT_POINT_POS){}
	CollisionPoint(gmtl::Point3d p, gmtl::Vec3d n) : normal(n), position(p){}
	CollisionPoint(gmtl::Point3d p, gmtl::Vec3d n, MatSample mat) : normal(n), position(p), material(mat) {}

	gmtl::Vec3d getNormal() {
		return gmtl::Vec3d(normal);
	}

	gmtl::Point3d getPosition() {
		return gmtl::Point3d(position);
	}

	MatSample getMaterial() {
		return material;
	}
};

class RayCollisionResult {	//TODO: add unique object ids to identify them when refracting
private:
	bool collided;
	CollisionPoint collisionPoint;
public:

	RayCollisionResult() : collided(false), collisionPoint(CollisionPoint()) {}
	RayCollisionResult(bool col, CollisionPoint colPoint) : collided(col), collisionPoint(colPoint){}
	RayCollisionResult(bool col) : collided(col), collisionPoint(CollisionPoint()) {}

	void setCollided(bool col)
	{
		collided = col;
	}
	void setCollisionPoint(CollisionPoint colPoint) {
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