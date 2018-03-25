#pragma once
#include <GL\freeglut.h>
#include <ParameterTypes.h>
/*
	DevILGLWindow

	Class that displays DevIL images on a window output
*/
class DevILGLWindow
{
private:
	double Width;
	double Height;
	GLuint window;
	void init(int argc, char** argv);
public:
	DevILGLWindow() {} // BOILER PLATE - DO NOT USE
	DevILGLWindow(int argc, char** argv) { init(argc, argv); }
	~DevILGLWindow() {}
	void setWidth(double width) { Width = width; }
	void setHeight(double height) { Height = height; }

	GLuint notifyTexture(GLuint TexID);
};