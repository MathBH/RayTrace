#include "RayCam.h"
#include <gmtl\gmtl.h>
#include <iostream>
#include "Scene.h"
#include "SphereTraceable.h"

// TODO: find a library to include for visual output and that can go get image data somewhere in memory and do wtv with it

int main(void) {
	SphereTraceable s = SphereTraceable();
	std::cout << "Demo Tool\n";

	gmtl::Point3d p1 = gmtl::Point3d(0.0, 0.0, 0.0);
	gmtl::Point3d p2 = gmtl::Point3d(p1);
	p2.set(1.0, 1.0, 1323.0);

	std::cout << "[" << p1[0] << ", " << p1[1] << ", " << p1[2] << "] \n"
		<< "[" << p2[0] << ", " << p2[1] << ", " << p2[2] << "] \n";
	return 0;
}