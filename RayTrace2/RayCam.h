#pragma once
#include <gmtl\Point.h>
#include <gmtl\Vec.h>
#include <gmtl\Ray.h>
#include <gmtl\Generate.h>
#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif
#include <math.h>

#ifndef DEFAULT_FOV
#define DEFAULT_FOV (33.0 * M_PI)/180.
#endif
#ifndef DEFAULT_D_NEAR				// Distance of the near Plane from the camera
#define DEFAULT_D_NEAR 0.1
#endif
#ifndef DEFAULT_ANTIALIAS
#define DEFAULT_ANTIALIAS 16
#endif
#ifndef DEFAULT_ASPECT_RATIO
#define DEFAULT_ASPECT_RATIO 1.33
#endif
#ifndef DEFAULT_RESOLUTION_WIDTH
#define DEFAULT_RESOLUTION_WIDTH 640
#endif
#ifndef DEFAULT_RESOLUTION_HEIGHT
#define DEFAULT_RESOLUTION_HEIGHT 480
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

class ResolutionSettings {
public:
	int width;
	int height;

	ResolutionSettings() : width(DEFAULT_RESOLUTION_WIDTH), height(DEFAULT_RESOLUTION_HEIGHT) {}
	ResolutionSettings(int w, int h) : width(w), height(h){}
};

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
		x0 = -(width / 2);
		y0 = -(height / 2);
	}
};

class RayIterator
{
protected:
	gmtl::Point3d cameraPos;
	ProjectionPlane projectionPlane;
	int xFullScale;
	int yFullScale;
	int xCursor;
	int yCursor;
	
	RayIterator(gmtl::Point3d camPos, ProjectionPlane projPlane,
		ResolutionSettings resolutionSettings, int antiAlias)
		: xCursor(0), yCursor(0), xFullScale(resolutionSettings.width*antiAlias),
		yFullScale(resolutionSettings.height*antiAlias), cameraPos(camPos), projectionPlane(projPlane){}

	gmtl::Point3d getPointOnPlane(double xVal, double yVal) {
		double x = projectionPlane.x0 + projectionPlane.width * xVal;
		double y = projectionPlane.y0 + projectionPlane.height * yVal;
		return gmtl::Point3d(x, y, projectionPlane.z);
	}

public:
	friend class RayCam;
	~RayIterator() {}

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
	~RayCam() {}

	void setFov(double newFov)
	{
		//projectionPlane = newFov;
	}
	void setZNear(double newZNear)
	{
		//zNear = newZNear;
	}
	void setAntiAlias(int newAntiAlias)
	{
		antiAlias = newAntiAlias;
	}
	void setAspectRatio(double newAspectRatio)
	{
		//aspectRatio = newAspectRatio;
	}
	void setResolution(ResolutionSettings resSet) {
		resolutionSettings = resSet;
	}
	RayIterator iterator() {
		return RayIterator(position, projectionPlane, resolutionSettings, antiAlias);
	}
};