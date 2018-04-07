#pragma once
#include <gmtl\Point.h>
#include <gmtl\Vec.h>
#include <gmtl\Ray.h>
#include <gmtl\Generate.h>
#include "Camera.h"
#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif
#include <math.h>
#include "ParameterTypes.h"
#include <vector>

#ifndef DEFAULT_FOV
#define DEFAULT_FOV (33.0 * M_PI)/180.
#endif
#ifndef DEFAULT_D_NEAR				// Distance of the near Plane from the camera
#define DEFAULT_D_NEAR 0.1
#endif
#ifndef DEFAULT_ANTIALIAS
#define DEFAULT_ANTIALIAS 1
#endif
#ifndef DEFAULT_ASPECT_RATIO
#define DEFAULT_ASPECT_RATIO DEFAULT_RESOLUTION_WIDTH/DEFAULT_RESOLUTION_HEIGHT
#endif
#ifndef DEFAULT_CAM_POS
#define DEFAULT_CAM_POS gmtl::Point3d(0.0,0.0,0.0)
#endif
#ifndef DEFAULT_CAM_FORWARD
#define DEFAULT_CAM_FORWARD gmtl::Vec3d(0.0,0.0,-1.0)
#endif
#ifndef DEFAULT_CAM_UP
#define DEFAULT_CAM_UP gmtl::Vec3d(0.0,1.0,0.0)
#endif

class ProjectionPlane {
public:
	double x0;
	double y0;
	double z;
	double width;
	double height;

	ProjectionPlane() : ProjectionPlane(DEFAULT_FOV, DEFAULT_D_NEAR, DEFAULT_ASPECT_RATIO) {}
	ProjectionPlane(double fov, double d, double aspectRatio)
	{
		z = -d;
		width = abs(z * tan(fov)) * 2;
		height = width / aspectRatio;
		x0 = -(width / 2.0);
		y0 = -(height / 2.0);
	}
};

/*
	Container class for holding a set of rays
*/
class RayPacket
{
	std::vector<Rayd> data;
	int beginPosition;
	int endPosition;

	class Iterator {
		std::vector<Rayd>& data;
		int position;
	public:
		Iterator(std::vector<Rayd>& _data, int _position) : data(_data), position(_position) {}
		Rayd& operator*() { return data[position]; }
		Iterator& operator++() { position++ ; return *this; }
		bool operator!=(const Iterator& iterator) { return position != iterator.position; }
	};

public:
	RayPacket() : beginPosition(0), endPosition(0) { data = std::vector<Rayd>(); }
	~RayPacket() {}
	
	void add(Rayd ray) { data.push_back(ray); endPosition++; }
	Iterator begin() { return Iterator(data, beginPosition); }
	Iterator end() { return Iterator(data, endPosition); }
};

// TODO: refactor and rename as this no longer acts like an iterator
class RayIterator
{
protected:
	gmtl::Point3d cameraPos;
	ProjectionPlane projectionPlane;
	int xFullScale;
	int yFullScale;
	int xCursor;
	int yCursor;
	int AntiAlias;
	
	RayIterator(gmtl::Point3d camPos, ProjectionPlane projPlane,
		ResolutionSettings resolutionSettings, int antiAlias)
		: xCursor(0), yCursor(0), xFullScale(resolutionSettings.width*antiAlias),
		yFullScale(resolutionSettings.height*antiAlias), cameraPos(camPos), projectionPlane(projPlane), AntiAlias(antiAlias){}

	gmtl::Point3d getPointOnPlane(double xVal, double yVal) {
		double x = projectionPlane.x0 + projectionPlane.width * xVal;
		double y = projectionPlane.y0 + projectionPlane.height * yVal;
		return gmtl::Point3d(x, y, projectionPlane.z);
	}

public:
	friend class RayCam;	//TODO: rotate rays so they are aligned with cam direction
	~RayIterator() {}

	RayPacket getAt(int x, int y) {
		RayPacket rayPacket = RayPacket();
		int xCursor = x* AntiAlias;
		int yCursor = y* AntiAlias;
		//TODO: add guards for out of bounds x and y values

		//gmtl::Point3d pointOnPlane = getPointOnPlane(xCursor, yCursor); // get point on the plane,
		//std::cout << "\nprojection: [" << pointOnPlane[0] << " , " << pointOnPlane[1] << " , " << pointOnPlane[2] << "]";
		
		for (int yDelta = 0; yDelta < AntiAlias; yDelta++) {
			for (int xDelta = 0; xDelta < AntiAlias; xDelta++) {
				gmtl::Rayd ray = gmtl::Rayd();
				double xPos = (xCursor + xDelta) / (double)xFullScale;
				double yPos = (yCursor + yDelta) / (double)yFullScale;
				gmtl::Point3d pointOnPlane = getPointOnPlane(xPos, yPos); // get point on the plane,
				gmtl::Vec3d rayDir = gmtl::makeNormal(gmtl::Vec3d(pointOnPlane)); // since the plane is centered on y and x and z from the center, this is bassically a direction vector
				ray.setOrigin(cameraPos);
				ray.setDir(rayDir);
				rayPacket.add(ray);
			}
		}
		return rayPacket;
	}

	bool hasNext()
	{ 
		return yCursor < yFullScale;
	}
	gmtl::Rayd next()
	{
		gmtl::Rayd ray = gmtl::Rayd();

		double xPos = xCursor / (double)xFullScale;
		double yPos = yCursor / (double)yFullScale;
		gmtl::Point3d pointOnPlane = getPointOnPlane(xPos, yPos); // get point on the plane,
		gmtl::Vec3d rayDir = gmtl::makeNormal(gmtl::Vec3d(pointOnPlane)); // since the plane is centered on y and x and z from the center, this is bassically a direction vector
		ray.setOrigin(cameraPos);
		ray.setDir(rayDir);

		// Increment x, y indices
		xCursor++;
		if (xCursor >= xFullScale)
		{
			xCursor = 0;
			yCursor ++;
		}

		return ray;
	}
};


class RayCam
{
protected:
	ResolutionSettings resolutionSettings;
	ProjectionPlane projectionPlane;
	int antiAlias;

	void init()
	{
		resolutionSettings = ResolutionSettings();
		projectionPlane = ProjectionPlane();
		antiAlias = DEFAULT_ANTIALIAS;
	}

public:
	gmtl::Point3d position;
	gmtl::Vec3d forward;
	gmtl::Vec3d up;

	RayCam() : position(DEFAULT_CAM_POS), forward(DEFAULT_CAM_FORWARD), up(DEFAULT_CAM_UP)
	{
		init();
	}

	RayCam(gmtl::VecBase<double, 3> p, gmtl::VecBase<double, 3> f, gmtl::VecBase<double, 3> u) : position(p), forward(f), up(u)
	{
		init();
	}

	RayCam(Camera cam, double zNear, double fov, int antAl, ResolutionSettings resSet) : position(cam.position), forward(cam.forward), up(cam.up) {
		double aspRat = resSet.width / resSet.height;

		resolutionSettings = resSet;
		projectionPlane = ProjectionPlane(fov, zNear, aspRat);
		antiAlias = antAl;
	}

	~RayCam() {}

	//void setFov(double newFov)
	//{
	//	//projectionPlane = newFov;
	//}
	//void setZNear(double newZNear)
	//{
	//	//zNear = newZNear;
	//}
	//void setAntiAlias(int newAntiAlias)
	//{
	//	antiAlias = newAntiAlias;
	//}
	//void setAspectRatio(double newAspectRatio)
	//{
	//	//aspectRatio = newAspectRatio;
	//}
	//void setResolution(ResolutionSettings resSet) {
	//	resolutionSettings = resSet;
	//}
	RayIterator iterator() {
		return RayIterator(position, projectionPlane, resolutionSettings, antiAlias);
	}
};