#include "RayCam.h"
#include <gmtl\gmtl.h>
#include <iostream>
#include "Scene.h"
#include "SphereTraceable.h"
#include "RTODevIL.h"

// TODO: find a library to include for visual output and that can go get image data somewhere in memory and do wtv with it

int main(void) {
	RTODevIL output = RTODevIL();
	for (int y = 0; y < 200; y++) {
		for (int x = 0; x < 200; x++) {
			output.setPixel(x, y, ColorRGB(0.5, 0.2, 0.5));
		}
	}

	for (int y = 200; y < 250; y++) {
		for (int x = 150; x < 200; x++) {
			output.setPixel(x, y, ColorRGB(1., 0.2, 0.7));
		}
	}
	std::cout << output.writeToFile("butt19.png");
	SphereTraceable s = SphereTraceable();
	std::cout << "Demo Tool\n";

	gmtl::Point3d p1 = gmtl::Point3d(0.0, 0.0, 0.0);
	gmtl::Point3d p2 = gmtl::Point3d(p1);
	p2.set(1.0, 1.0, 1323.0);

	std::cout << "[" << p1[0] << ", " << p1[1] << ", " << p1[2] << "] \n"
		<< "[" << p2[0] << ", " << p2[1] << ", " << p2[2] << "] \n";
	return 0;
}