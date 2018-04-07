#pragma once
#include <gmtl/Vec.h>
#include <gmtl/AxisAngle.h>
#include <gmtl/Matrix.h>
#include <gmtl/Generate.h>
#define X_AXIS gmtl::Vec3d(1.0,0.0,0.0)
#define Y_AXIS gmtl::Vec3d(0.0,1.0,0.0)
#define Z_AXIS gmtl::Vec3d(0.0,0.0,1.0)
#include <iostream>


gmtl::Vec3d rotateVector(double azm, double elev, gmtl::Vec3d vector) {
	gmtl::AxisAngled elevation = gmtl::AxisAngled(elev, X_AXIS);
	gmtl::AxisAngled azimut = gmtl::AxisAngled(azm, Y_AXIS);

	gmtl::Matrix44d elevationRotation;
	gmtl::Matrix44d azimutRotation;

	gmtl::setRot(elevationRotation, elevation);
	gmtl::setRot(azimutRotation, azimut);

	vector = elevationRotation * vector;
	vector = azimutRotation * vector;
	vector = gmtl::makeNormal(vector);

	return vector;
}

double getElevation(gmtl::VecBase<double,3> vector) {
	//std::cout << "\nvec: " << vector[0] << " " << vector[1] << " " << vector[2];
	double radius = gmtl::length<double,3>(vector);
	double elevation = acos(vector[1]/radius);
	if (vector[2] < 0.) { elevation = -elevation; }
	//std::cout << "\nelev: " << elevation;
	return elevation;
}

double getAzimut(gmtl::VecBase<double, 3> vector) {
	//std::cout << "\nvec: " << vector[0] << " " << vector[1] << " " << vector[2];
	double azimut = atan(vector[0]/vector[2]);
	//std::cout << "\nazimut: " << azimut;
	return azimut;
}