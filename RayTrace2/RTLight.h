#pragma once
#include <gmtl/Ray.h>
#include <gmtl/Point.h>
#include <gmtl/Plane.h>
#include "ParameterTypes.h"
#include <gmtl/Generate.h>

class Light
{
private:
	double FalloffA;
	double FalloffB;
	double FalloffC;
	ColorRGB Diffuse;
	ColorRGB Specular;
	gmtl::Point3d Position;

public:
	Light() {}
	Light(gmtl::Point3d pos) : Position(pos) {}
	Light(gmtl::Point3d pos, ColorRGB diffuse, ColorRGB specular, double falloffa, double falloffb, double falloffc) : Position(pos), Diffuse(diffuse), Specular(specular), FalloffA(falloffa), FalloffB(falloffb), FalloffC(falloffc){}
	~Light() {}

	void setPosition(gmtl::Point3d pos) { Position = pos; }
	gmtl::Point3d getPosition() { return Position; }

	void setDiffuse(ColorRGB diffuse) { Diffuse = diffuse; }
	ColorRGB getDiffuse() { return Diffuse; }

	void setSpecular(ColorRGB specular) { Specular = specular; }
	ColorRGB getSpecular() { return Specular; }

	void setFalloff(double a, double b, double c) { FalloffA = a; FalloffB = b; FalloffC = c; }
	double getFalloff(gmtl::Point3d objPos)
	{
		gmtl::Vec3d pathBetween = Position - objPos;
		double distance = gmtl::length(pathBetween);
		distance = abs(distance);
		//std::cout << "\nHI: " << (1. / (pow((distance*FalloffA), 2.) + (distance*FalloffB) + FalloffC));
		return (1. / (pow((distance*FalloffA),2.) + (distance*FalloffB) + FalloffC));
	}
};