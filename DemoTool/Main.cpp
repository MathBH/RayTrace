#include "RayCam.h"
#include <gmtl\gmtl.h>
#include <iostream>
#include "Scene.h"
#include "SphereTraceable.h"
#include "RTODevIL.h"
#include "RayTracer.h"
#include "DevILImage.h"
#include <SkyMap.h>

// TODO: find a library to include for visual output and that can go get image data somewhere in memory and do wtv with it

int main(int argc, char** argv) {
	//for (int y = 0; y < 200; y++) {
	//	for (int x = 0; x < 200; x++) {
	//		output.setPixel(x, y, ColorRGB(0.5, 0.2, 0.5));
	//	}
	//}
	//
	//for (int y = 200; y < 250; y++) {
	//	for (int x = 150; x < 200; x++) {
	//		output.setPixel(x, y, ColorRGB(1., 0.2, 0.7));
	//	}
	//}
	//std::cout << output.writeToFile("butt19.png");
	//SphereTraceable s = SphereTraceable();
	std::cout << "Demo Tool\n";

	// Setup Output
	RTODevIL output = RTODevIL();
	output.setFilePath("renderMirror2.png");

	// Setup Scene
	DevILImage skyMapData = DevILImage("skymaps/AboveTheSea.jpg");
	SkyMap skyMap = SkyMap();
	//skyMap.setForward(gmtl::Vec3d(1., 0., 0.));
	//skyMap.setRight(gmtl::Vec3d(0., 0., 1.));
	skyMap.setImageData(skyMapData.getImageData(),skyMapData.getWidth(), skyMapData.getHeight());

	RTScene scene = RTScene();
	scene.camera = Camera();
	scene.camera.position = gmtl::Point3d(0.01,1.0,5.0);
	scene.Sky = skyMap;
	SphereTraceable sphere1 = SphereTraceable(Point3d(0.0, 0.0, -6.0), 2., RTMaterial(ColorRGB(0.0,0.8,0.4),0.5));
	SphereTraceable sphere2 = SphereTraceable(Point3d(-2.9, -0.3, -7.0), 0.5, RTMaterial(ColorRGB(0.0, 1.0, 0.7), 0.5));
	SphereTraceable sphere3 = SphereTraceable(Point3d(-1.0, 2.3, -7.0), 0.5, RTMaterial(ColorRGB(0.0,1.0,0.0),0.5));
	SphereTraceable sphere4 = SphereTraceable(Point3d(-2.5, 1.3, -6.0), 1.0, RTMaterial(ColorRGB(0.1,0.4,0.7),0.5));
	SphereTraceable sphere5 = SphereTraceable(Point3d(2.5, -1.3, -6.0), 1.0, RTMaterial(ColorRGB(0.0,1.0,1.0),0.5));
	SphereTraceable sphere6 = SphereTraceable(Point3d(2.9, 0.3, -7.0), 0.5, RTMaterial(ColorRGB(1.0, 1.0, 1.0), 0.5));
	SphereTraceable sphere7 = SphereTraceable(Point3d(1.0, -2.3, -7.0), 0.5, RTMaterial(ColorRGB(0.5, 1.0, 0.0), 0.5));
	SphereTraceable sphere8 = SphereTraceable(Point3d(0.0, 0.0, -15.0), 8., RTMaterial(ColorRGB(0.3, 0.6, 0.7), 0.5));
	scene.objects.push_front(&sphere1);
	scene.objects.push_front(&sphere2);
	scene.objects.push_front(&sphere3);
	scene.objects.push_front(&sphere4);
	scene.objects.push_front(&sphere5);
	scene.objects.push_front(&sphere6);
	scene.objects.push_front(&sphere7);
	scene.objects.push_front(&sphere8);

	//RTLight light1 = RTLight(Point3d(1.0,1.0,4.0));
	//scene.lights.push_front(&light1);

	RTSettings renderSettings = RTSettings();
	renderSettings.antiAlias = 16;
	renderSettings.resolution = ResolutionSettings(850,480);
	renderSettings.camSettings.fov = DEFAULT_FOV;
	renderSettings.camSettings.zNear = DEFAULT_D_NEAR;
	RayTracer rayTracer = RayTracer();
	rayTracer.setAmbientColor(ColorRGB(1.0, 0.7, 0.5));
	rayTracer.setRenderSettings(renderSettings);
	rayTracer.setOutput(&output);
	rayTracer.setScene(&scene);
	rayTracer.render();
	output.commit();

	//gmtl::Point3d p1 = gmtl::Point3d(0.0, 0.0, 0.0);
	//gmtl::Point3d p2 = gmtl::Point3d(p1);
	//p2.set(1.0, 1.0, 1323.0);

	//std::cout << "[" << p1[0] << ", " << p1[1] << ", " << p1[2] << "] \n"
	//	<< "[" << p2[0] << ", " << p2[1] << ", " << p2[2] << "] \n";
	return 0;
}