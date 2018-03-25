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
		RayCollisionResult nearestCollisionResult = RayCollisionResult(false);

		for (TraceableObject * object : objects)
		{
			RayCollisionResult result = object->tryCollision(ray);
			if (result.getCollided())
			{
				if (!foundCollision)
				{
					nearestCollisionResult = result;
					foundCollision = true;
				}
				else
				{
					// Get the vectors betweenn the collision points and the ray's origin
					Vec3d colToRay = result.getCollisionPoint().getPosition() - ray.getOrigin();
					Vec3d nearestestColToRay = nearestCollisionResult.getCollisionPoint().getPosition() - ray.getOrigin();
					// Compare the length of these vectors : this thus gives the distance from the collision
					// to the ray and allows you to pick the first object in the way, ignoring the others
					if (gmtl::length(colToRay) < gmtl::length(nearestestColToRay))
					{
						nearestCollisionResult = result;
					}
				}
			}
		}
		return nearestCollisionResult;
	}
};