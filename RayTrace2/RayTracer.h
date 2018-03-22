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

class RayTracer
{
private:
	RTOutput* renderOutput;
	Scene* scene;
	RTSettings renderSettings;

	bool sceneSet;
	bool outputSet;


	//Helper function - TODO: refactor
	void insertBufferLine(vector<ColorRGB> pixelBuffer, int yIndex);

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
	int setScene(Scene * scn);

	/*
	Run render
	return:
		>=0 on success
		negative values otherwise
	*/
	int render();
};

