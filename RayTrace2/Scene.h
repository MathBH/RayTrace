#pragma once
#include <forward_list>
#include "Camera.h"
#include "TraceableObject.h"
#include "RTLight.h"

/*
	Class: Scene

	Holds all items found in a scene and provides an interface for accessing and adding them.
*/
class Scene {
private:
	/*
		Holder for scene objects (as opposed to lights or cameras)
	*/
	class Objects {
	public:
		std::forward_list<TraceableObject*> traceable;
		Objects() {}
		~Objects() {}
	};

public:
	Camera camera;
	Objects objects;
	std::forward_list<RTLight*> rayTraceLights;
	Scene() {}
};