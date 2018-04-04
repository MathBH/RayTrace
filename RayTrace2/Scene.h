#pragma once
#include <forward_list>
#include "Camera.h"
#include "TraceableObject.h"
#include "RTLight.h"
#include "SkyMap.h"

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

/*
Class: RTScene

Holds all items found in a ray traceable scene and provides an interface for accessing and adding them.
*/
class RTScene {
public:
	ColorRGB ambientColor;
	Camera camera;
	SkyMap Sky;
	std::forward_list<RTLight*> lights;
	std::forward_list<TraceableObject*> objects;
	RTScene() {}
	
	/*
		Try Collision:

		Move the ray through the scene and return a report describing
		the first collision it encountered
	*/
	RayCollisionResult tryCollision(const gmtl::Rayd ray) //TODO: refactor
	{
		bool foundCollision = false;
		RayCollisionResult output = RayCollisionResult(false);
		double shortestDistance = INFINITY;

		for (TraceableObject * object : objects)
		{
			RayCollisionResult colResult = object->tryCollision(ray);
			if (colResult.getCollided())
			{
				// Get the vectors betweenn the collision points and the ray's origin
				Vec3d colToRay = colResult.getCollisionPoint().getPosition() - ray.getOrigin();
				double colToRayLen = gmtl::length(colToRay);
				// Compare the length of these vectors : this thus gives the distance from the collision
				// to the ray and allows you to pick the first object in the way, ignoring the others
				if (colToRayLen < shortestDistance)
				{
					shortestDistance = colToRayLen;
					output = colResult;
				}
			}
		}
		return output;
	}
};