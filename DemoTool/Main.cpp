#include "RayCam.h"
#include <gmtl\gmtl.h>
#include <iostream>
#include "Scene.h"
#include "SphereTraceable.h"
#include "RTODevIL.h"
#include "RayTracer.h"
#include "DevILImage.h"
#include <SkyMap.h>
#include "DevILTexture.h"
#include <DiskTraceable.h>
#include <FlatMapping.h>

#define METAL_SPHERE_HD "metalSpheres"



int demoHighRes(string outputPath) {
	RTODevIL output = RTODevIL();
	output.setFilePath(outputPath);

	// Setup Scene
	DevILImageRGB skyMapData = DevILImageRGB("skymaps/AboveTheSea.jpg");
	SkyMap skyMap = SkyMap();
	RTLight light1 = RTLight(gmtl::Point3d(-7., 1., -7.0), ColorRGB(3.8, 3.8, 3.5), ColorRGB(8, 8, 5), 1., 0.7, 1.);
	RTLight light2 = RTLight(gmtl::Point3d(7., 1., -2.0), ColorRGB(3.8, 3.8, 3.5), ColorRGB(8, 8, 5), 1., 0.7, 1.);
	skyMap.setImageData(skyMapData.getImageData(), skyMapData.getWidth(), skyMapData.getHeight());
	skyMap.setAzimutOffset(-(1.25*M_PI) / 3);
	skyMap.setElevationOffset(-M_PI / 32);
	RTScene scene = RTScene();
	scene.camera = Camera();
	scene.camera.position = gmtl::Point3d(0.0, 0.0, 5.0);
	scene.Sky = skyMap;
	scene.ambientColor = ColorRGB(0.062, 0.051, 0.07);

	BasicMaterial metalMat = BasicMaterial();
	metalMat.setDiffuse(ColorRGB(6.0, 7.1, 7.0));
	metalMat.setSpecular(ColorRGB(8.5, 8.5, 8.5));
	metalMat.setAbsorbtion(ColorRGB(0.15, 0.15, 0.19));
	metalMat.setRefractiveIndex(8.11);
	metalMat.setReflectivity(0.3);
	metalMat.setShininess(32);
	//MatSample waterMat = MatSample(ColorRGB(1.0, 1.0, 1.0), ColorRGB(0.15, 0.15, 0.19), 8.11, 0.3);
	//MatSample otherMat = MatSample(ColorRGB(1.0, 1.0, 1.0), ColorRGB(1., 1., 1.), 1.56, 0.5);
	//otherMat.setDiffuse(ColorRGB(20.0, 20.0, 20.0));
	//otherMat.setSpecular(ColorRGB(0., 0., 0.));
	//otherMat.setShininess(3);


	DevILImageRGB _checkerDiffuse = DevILImageRGB("textures/checkerPattern.jpg");
	DevILTexture checkerDiffuse = DevILTexture();
	checkerDiffuse.setSource(&_checkerDiffuse);
	TextureMaterial checkerMat = TextureMaterial();

	checkerMat.setDiffuseMap(&checkerDiffuse);
	//checkerMat.setAmbientMap(&checkerDiffuse);
	checkerMat.setAmbient(ColorRGB(0.2, 0.28, 0.3));
	checkerMat.setDiffuse(ColorRGB(8.9, 9.1, 9.0));
	checkerMat.setSpecular(ColorRGB(1.5, 1.5, 1.5));
	checkerMat.setAbsorbtion(ColorRGB(0.15, 0.15, 0.19));
	checkerMat.setRefractiveIndex(1.12);
	checkerMat.setReflectivity(0.04);
	checkerMat.setShininess(19);

	DevILImageRGB _barkDiffuse = DevILImageRGB("textures/Bark.jpg");
	DevILTexture barkDiffuse = DevILTexture();
	barkDiffuse.setSource(&_barkDiffuse);

	DevILImageRGB _barkNormal = DevILImageRGB("textures/BarkN.jpg");
	DevILTexture barkNormal = DevILTexture();
	barkNormal.setSource(&_barkNormal);

	TextureMaterial woodMat = TextureMaterial();
	//woodMat.setDiffuseMap(&barkDiffuse);
	//woodMat.setSpecularMap(&barkDiffuse);
	//woodMat.setAmbientMap(&barkDiffuse);

	//woodMat.setNormalMap(&barkNormal);
	//woodMat.setNormalWeight(1.);

	woodMat.setAmbient(ColorRGB(0.3, 0.3, 0.3));
	woodMat.setDiffuse(ColorRGB(1.5, 1.5, 1.5));
	woodMat.setSpecular(ColorRGB(1.5, 1.5, 1.5));
	woodMat.setAbsorbtion(ColorRGB(0.15, 0.15, 0.19));
	woodMat.setRefractiveIndex(1.57);
	woodMat.setReflectivity(0.0);
	woodMat.setShininess(2);

	double standRadius = 4.0;

	FlatMapping flatMap = FlatMapping(gmtl::Vec3d(1., 0., 0.), gmtl::Vec3d(0., 0., 1.), standRadius*2., standRadius*2.);
	SphericalMapping sphereMap = SphericalMapping();

	StandTraceable stand = StandTraceable(Point3d(0., -2., -7.), standRadius, &checkerMat);
	SphereTraceable sphere1 = SphereTraceable(Point3d(0, 0, -6.0), 2., &metalMat);
	SphereTraceable sphere2 = SphereTraceable(Point3d(3., -1., -7.0), 1., &metalMat);
	SphereTraceable sphere3 = SphereTraceable(Point3d(-1.0, 2.3, -7.0), 0.5, &metalMat);
	SphereTraceable sphere4 = SphereTraceable(Point3d(-3.0, 1.3, -7.0), 1.0, &metalMat);
	SphereTraceable sphere8 = SphereTraceable(Point3d(-6.0, 0.0, -15.0), 8., &metalMat);

	stand.setMapping(&flatMap);
	sphere1.setMapping(&sphereMap);
	sphere2.setMapping(&sphereMap);
	sphere3.setMapping(&sphereMap);
	sphere4.setMapping(&sphereMap);
	//sphere8.setMapping(&sphereMap);
	//SphereTraceable sphere5 = SphereTraceable(Point3d(2.5, -1.3, -6.0), 1.0, RTMaterial(ColorRGB(0.0,1.0,1.0), 1.33));
	//SphereTraceable sphere6 = SphereTraceable(Point3d(2.9, 0.3, -7.0), 0.5, RTMaterial(ColorRGB(1.0, 1.0, 1.0), 1.33));
	//SphereTraceable sphere7 = SphereTraceable(Point3d(1.0, -2.3, -7.0), 0.5, RTMaterial(ColorRGB(0.5, 1.0, 0.0), 1.33));
	scene.objects.push_front(&stand);
	scene.objects.push_front(&sphere1);
	scene.objects.push_front(&sphere2);
	scene.lights.push_front(&light1);
	scene.lights.push_front(&light2);
	//scene.objects.push_front(&sphere3);
	//scene.objects.push_front(&sphere4);
	//scene.objects.push_front(&sphere5);
	//scene.objects.push_front(&sphere6);
	//scene.objects.push_front(&sphere7);
	//scene.objects.push_front(&sphere8);

	RTSettings renderSettings = RTSettings();
	renderSettings.antiAlias = 1;
	renderSettings.resolution = ResolutionSettings(425, 240);
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

	return 0;
}

int main(int argc, char** argv) {
	// do a thing that lists a bunch of demos that you choose by name and type out the output file name
	string inputBuffer;
	std::cout << "Demo Tool\n";
	while (true) {
		std::cout << "\nAvailable Demos\n---------------\n\n- " << METAL_SPHERE_HD << "\n\n";
		std::cin >> inputBuffer;

		if (inputBuffer.compare(METAL_SPHERE_HD) == 0)
		{
			std::cout << "\nPlease specify output path (from DemoTool source)\n";
			std::cin >> inputBuffer;
			return demoHighRes(inputBuffer);
		}
	}
	return 0;
}