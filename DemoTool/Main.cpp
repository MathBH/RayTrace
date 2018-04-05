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
	// do a thing that lists a bunch of demos that you choose by name and type out the output file name


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
	output.setFilePath("metalHD.png");

	// Setup Scene
	DevILImageRGB skyMapData = DevILImageRGB("skymaps/AboveTheSea.jpg");
	//DevILImageRGB skyMapLightData = DevILImageRGB("skymaps/AboveTheSea_L.jpg");
	SkyMap skyMap = SkyMap();
	RTLight light1 = RTLight(gmtl::Point3d(-5., 1., 2.0), ColorRGB(3.8, 3.8, 3.5), ColorRGB(8, 8, 5), 1., 1., 1.);
	//skyMap.setForward(gmtl::Vec3d(1., 0., 0.));
	//skyMap.setRight(gmtl::Vec3d(0., 0., 1.));
	//skyMap.setLightData(skyMapLightData.getImageData(), skyMapLightData.getWidth(), skyMapLightData.getHeight());
	skyMap.setImageData(skyMapData.getImageData(),skyMapData.getWidth(), skyMapData.getHeight());
	skyMap.setAzimutOffset(-(1.25*M_PI)/ 3);
	skyMap.setElevationOffset(-M_PI/32);
	RTScene scene = RTScene();
	scene.camera = Camera();
	scene.camera.position = gmtl::Point3d(0.0,0.0,5.0);
	scene.Sky = skyMap;
	scene.ambientColor = ColorRGB(0.062, 0.051, 0.07);

	RTMaterial waterMat = RTMaterial(ColorRGB(1.0, 1.0, 1.0), ColorRGB(0.15, 0.15, 0.19), 8.11, 0.3);
	RTMaterial otherMat = RTMaterial(ColorRGB(1.0, 1.0, 1.0), ColorRGB(1., 1., 1.), 1.56, 0.5);
	otherMat.setDiffuse(ColorRGB(20.0, 20.0, 20.0));
	otherMat.setSpecular(ColorRGB(0., 0., 0.));
	otherMat.setShininess(3);
	waterMat.setDiffuse(ColorRGB(6.0, 7.1, 7.0));
	waterMat.setSpecular(ColorRGB(8.5, 8.5, 8.5));
	waterMat.setShininess(32);

	SphereTraceable sphere1 = SphereTraceable(Point3d(0, 0, -6.0), 2., waterMat);
	//std::cout << "\nSphere pos: " << sphere1.getPosition()[0] << " " << sphere1.getPosition()[1] << " " << sphere1.getPosition()[2];
	SphereTraceable sphere2 = SphereTraceable(Point3d(3., -1., -7.0), 1., waterMat);
	SphereTraceable sphere3 = SphereTraceable(Point3d(-1.0, 2.3, -7.0), 0.5, waterMat);
	SphereTraceable sphere4 = SphereTraceable(Point3d(-3.0, 1.3, -7.0), 1.0, waterMat);
	//SphereTraceable sphere5 = SphereTraceable(Point3d(2.5, -1.3, -6.0), 1.0, RTMaterial(ColorRGB(0.0,1.0,1.0), 1.33));
	//SphereTraceable sphere6 = SphereTraceable(Point3d(2.9, 0.3, -7.0), 0.5, RTMaterial(ColorRGB(1.0, 1.0, 1.0), 1.33));
	//SphereTraceable sphere7 = SphereTraceable(Point3d(1.0, -2.3, -7.0), 0.5, RTMaterial(ColorRGB(0.5, 1.0, 0.0), 1.33));
	//SphereTraceable sphere8 = SphereTraceable(Point3d(0.0, 0.0, -15.0), 8., RTMaterial(ColorRGB(0.3, 0.6, 0.7), 1.33));
	scene.objects.push_front(&sphere1);
	scene.objects.push_front(&sphere2);
	scene.lights.push_front(&light1);
	scene.objects.push_front(&sphere3);
	scene.objects.push_front(&sphere4);
	//scene.objects.push_front(&sphere5);
	//scene.objects.push_front(&sphere6);
	//scene.objects.push_front(&sphere7);
	//scene.objects.push_front(&sphere8);

	//RTLight light1 = RTLight(Point3d(1.0,1.0,4.0));
	//scene.lights.push_front(&light1);

	RTSettings renderSettings = RTSettings();
	renderSettings.antiAlias = 8;
	renderSettings.resolution = ResolutionSettings(850,480);
	renderSettings.camSettings.fov = DEFAULT_FOV;
	renderSettings.camSettings.zNear = DEFAULT_D_NEAR;
	renderSettings.camSettings.RayLife = 8;
	RayTracer rayTracer = RayTracer();
	//rayTracer.setAmbientColor(ColorRGB(0.062, 0.051, 0.07));
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