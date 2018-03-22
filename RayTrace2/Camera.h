#pragma once
#include <gmtl/Point.h>
#include <gmtl/Vec.h>
#ifndef DEFAULT_CAM_POS
#define DEFAULT_CAM_POS gmtl::Point3d(0.0,0.0,0.0)
#endif
#ifndef DEFAULT_CAM_FORWARD
#define DEFAULT_CAM_FORWARD gmtl::Vec3d(0.0,0.0,-1.0)
#endif
#ifndef DEFAULT_CAM_UP
#define DEFAULT_CAM_UP gmtl::Vec3d(0.0,1.0,0.0)
#endif
using namespace gmtl;

class Camera {
public:
	Point3d position;
	Vec3d forward;
	Vec3d up;

	Camera(): position(DEFAULT_CAM_POS), forward(DEFAULT_CAM_FORWARD), up(DEFAULT_CAM_UP) {}
	Camera(VecBase<double, 3> p, VecBase<double, 3> f, VecBase<double, 3> u) : position(p), forward(f), up(u) {}
	~Camera() {}
};