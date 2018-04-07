#include "SphereTraceable.h"
#include <gmtl/intersection.h>
#include <gmtl/Generate.h>
#include "Transformations.h"
#define EPSILON 0.000000001 // custom defined for now for the sake of control

RayCollisionResult SphereTraceable::tryCollision(gmtl::Rayd ray)
{
	RayCollisionResult result = RayCollisionResult(false);
	if (MyUVMap == nullptr || MyMaterial == nullptr) { return result; }
	int numHits;
	double t0, t1, t;

	if (gmtl::intersect<double>(SphereData, ray, numHits, t0, t1)) {
		t = (numHits > 1 && t1 < t0 && t1 > 0) ? t1 : t0;
		if (t <= 0) { return result; }

		gmtl::Point3d colPos = ray.getOrigin() + ray.getDir() * t;
		gmtl::Point3d relativePos = colPos - SphereData.getCenter();

		UVCoord coord = MyUVMap->getUV(relativePos);
		MatSample matSample = MyMaterial->getSample(coord);

		gmtl::Vec3d normal = gmtl::makeNormal(gmtl::Vec3d(relativePos));

		// ------------------------------------------------------------------
		//						 Normal Map WIP
		//
		//		if (matSample.normalIsActive())
		//		{
		//			double normalMapWeight = matSample.getNormalWeight();
		//			gmtl::Vec3d mapNormal = matSample.getNormal();
		//			getAzimut(gmtl::Vec3d(0., 1., 0.));
		//			double azimut = getAzimut(normal)*normalMapWeight;
		//			double elevation = getElevation(normal)*normalMapWeight;
		//			normal = rotateVector(azimut, elevation, mapNormal);
		//		}
		// ------------------------------------------------------------------

		CollisionPoint collisionPoint = CollisionPoint(colPos, normal, matSample);

		result.setCollided(true);
		result.setCollisionPoint(collisionPoint);
	}

	return result;
}