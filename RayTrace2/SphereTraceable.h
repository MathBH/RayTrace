#pragma once
#include <gmtl\Point.h>
#include <gmtl\Sphere.h>
#include "TraceableObject.h"
#include "HasMaterial.h"
#include "UVMapped.h"
#include "AllTextures.h"
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

class SphereTraceable : public TraceableObject, public HasMaterial, public UVMapped
{
protected:

	gmtl::Sphered SphereData;
	Material * MyMaterial;
	UVMap * MyUVMap;

public:
	SphereTraceable() : SphereTraceable(DEFAULT_SPHERE_POS, DEFAULT_SPHERE_RADIUS) {}
	SphereTraceable(gmtl::Point3d p, double r) : SphereData(gmtl::Sphered(p, r)) {}
	SphereTraceable(gmtl::Point3d p, double r, Material * material) : SphereData(gmtl::Sphered(p, r)), MyMaterial(material) {}
	SphereTraceable(gmtl::Point3d p, double r, Material * material, UVMap * uvMap) : SphereData(gmtl::Sphered(p, r)), MyMaterial(material), MyUVMap(uvMap) {}
	~SphereTraceable() {}

	double getRadius() { return SphereData.getRadius(); }
	gmtl::Point3d getPosition() { return SphereData.getCenter(); }

	void setRadius(double r) { SphereData.setRadius(r); }
	void setPosition(gmtl::Point3d p) { SphereData.setCenter(p); }

	void setMaterial(Material * material) override { MyMaterial = material; }
	void setMapping(UVMap * uvMap) override { MyUVMap = uvMap; }

	RayCollisionResult tryCollision(const gmtl::Rayd ray) override;
};