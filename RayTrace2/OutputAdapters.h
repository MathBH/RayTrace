#pragma once
/*
	Output Adapters
	---------------
	
	Tesla Belzile-Ha

	19/03/2018

	List of output adapters for the RayTracer to pass its data to.
*/

/*
	Adapter for a gil image
*/
#include <boost\gil\gil_all.hpp>
#include "Color.h"
#include <string>
using namespace boost::gil;
class GILAdapter
{
private:
	rgb8_image_t imageOut;
public:
	GILAdapter() {}
	GILAdapter(const rgb8_image_t imgo) : imageOut(imgo) {}
	~GILAdapter() {}
	int insertData(int x, int y, const ColorRGB& col) {
		//TODO: insert the data
		//		use the library to write col to the pixel position
		return 1;
	}
	int saveBmp(std::string filePath) {
		//TODO: save data
		//		use the library to save a bmp image
		return 1;
	}
};