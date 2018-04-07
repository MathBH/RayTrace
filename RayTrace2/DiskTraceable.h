#pragma once
#include <gmtl/Plane.h>
#include "TraceableObject.h"
#include "HasMaterial.h"
#include "UVMapped.h"
#define STAND_NORMAL gmtl::Vec3d(0.,1.,0.) //Stands are defined with a static upward orientation

class StandTraceable : public TraceableObject, HasMaterial, UVMapped
{
private:
	gmtl::Planed Plane;
	gmtl::Point3d Position;
	gmtl::Vec3d Normal = STAND_NORMAL;
	double Radius;

	UVMap * MyUVMap;
	Material * MyMaterial;

public:
	StandTraceable() {}
	StandTraceable(gmtl::Point3d position,  double radius) 
		: Radius(radius), Position(position), Plane(gmtl::Planed(STAND_NORMAL, position)) {}

	StandTraceable(gmtl::Point3d position, double radius, Material * material)
		: Radius(radius), Position(position), Plane(gmtl::Planed(STAND_NORMAL, position)), MyMaterial(material) {}
	~StandTraceable() {}

	RayCollisionResult tryCollision(const gmtl::Rayd ray) override;
	void setMaterial(Material * material) override { MyMaterial = material; }
	void setMapping(UVMap * uvMap) override { MyUVMap = uvMap; }
};

