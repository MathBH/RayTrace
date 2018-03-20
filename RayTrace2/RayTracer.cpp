#include "RayTracer.h"



RayTracer::RayTracer()
{
	rgb8_image_t image(4, 4);
	outputAdapter = GILAdapter(image);
}


RayTracer::~RayTracer()
{
}

int RayTracer::render()
{
	//TODO: write the actual ray tracing algorithm
	for (int y = 0; y < 400; y++) {
		for (int x = 0; x < 400; x++) {
			ColorRGB col = ColorRGB(0.5, 0.5, 0.0);
			outputAdapter.insertData(x, y, col);
		}
	}
	outputAdapter.saveBmp("firstImage.bmp");
	return 0;
}
