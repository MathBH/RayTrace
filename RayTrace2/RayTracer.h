#pragma once
#include "Scene.h"
#include "RTOutput.h"
#include "ParameterTypes.h"
#include <vector>

/*
	Ray Tracer

	Ray Tracing class

	TODO: add methods to set stuff
*/

class RTSettings {
public:
	class RayCamSettings { // TODO: make into an associable list of cam settings for multi cam
	public:
		double fov;
		double zNear;
		int RayLife;
		RayCamSettings() {}
		RayCamSettings(double f, double z): fov(f), zNear(z){}
		~RayCamSettings() {}
	};
	double antiAlias;
	ResolutionSettings resolution;
	RayCamSettings camSettings;

	RTSettings() {}
	RTSettings(ResolutionSettings resSet, double antAl) : resolution(resSet), antiAlias(antAl) {}
	~RTSettings() {};
};

//TODO: refactor and decide if to keep and move in header or find alternative
class RefractionResult {
public:
	double Distance;
	Rayd RefractedRay;
	RefractionResult(Rayd refractedRay, double distance) : RefractedRay(refractedRay), Distance(distance) {}
	~RefractionResult() {}
};

class RayTracer
{
private:
	RTOutput* renderOutput;
	RTScene* scene;
	RTSettings renderSettings;
	int RayLife;

	bool sceneSet;
	bool outputSet;


	//Helper function - TODO: refactor
	void insertBufferLine(vector<ColorRGB> pixelBuffer, int yIndex);
	
	ColorRGB evaluateDiffuse(gmtl::Point3d objPos, gmtl::Vec3d objNorm);
	ColorRGB evaluateSpec(gmtl::Point3d objPos, gmtl::Vec3d objNorm, MatSample material);
	std::vector<RefractionResult> refract(Rayd ray, CollisionPoint colPoint, CollisionPoint * lastCollision);

	ColorRGB trace(RTScene * scene, Rayd ray, int life, CollisionPoint * lastCollision = nullptr);

public:
	RayTracer() : sceneSet(false), outputSet(false) {}
	~RayTracer() {}

	/*
	Set render settings

	TODO:
	return:
		>=0 on success
		negative values for invalid settings
	*/
	void setRenderSettings(RTSettings rtSettings) {
		renderSettings = rtSettings;
	}

	/*
	Set target output
	return:
		>=0 on success
		negative values for invalid output
	*/
	int setOutput(RTOutput * rndrOut);

	/*
	Set scene to render
	return:
		>=0 on success
		negative values for invalid scene
	*/
	int setScene(RTScene * scn);

	/*
	Run render
	return:
		>=0 on success
		negative values otherwise
	*/
	int render();
};

