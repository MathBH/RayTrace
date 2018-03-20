#pragma once
#include <forward_list>
#include "RayCam.h"
#include "TraceableObject.h"

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

	//TODO add environment variables (like atmosphere values)


public:
	RayCam camera;	//TODO: refactor
	Objects objects;
	Scene() {}
};