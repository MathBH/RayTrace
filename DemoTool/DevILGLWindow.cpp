#include "DevILGLWindow.h"
#include <string>

double widthCache = 0;
double heightCache = 0;

void drawFunc(void)
{
	glClear(GL_COLOR_BUFFER_BIT);  // Clear the colour buffer.
	glBegin(GL_QUADS);

	glTexCoord2f(0.0f, 1.0f); glVertex3i(0, 0, 0);
	glTexCoord2f(1.0f, 1.0f); glVertex3i(widthCache, 0, 0);
	glTexCoord2f(1.0f, 0.0f); glVertex3i(widthCache, heightCache, 0);
	glTexCoord2f(0.0f, 0.0f); glVertex3i(0, heightCache, 0);
	glEnd();

	glutSwapBuffers();  // We use double buffering, so swap the buffers.
}

void DevILGLWindow::init(int argc, char** argv)
{
	
	glutInit(&argc, argv);  // Standard glut initialization.
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(Width, Height);

	widthCache = Width;
	heightCache = Height;

	window = glutCreateWindow("preview");
	glEnable(GL_TEXTURE_2D);  // Enable texturing.
	glMatrixMode(GL_PROJECTION);  // We want to use the projection matrix.
	glLoadIdentity();  // Loads the identity matrix into the current matrix.
	glOrtho(0, Width, Height, 0, -1, 1);  // Set up an orthographic projection with OpenGL.
	glutDisplayFunc(drawFunc);
}

GLuint DevILGLWindow::notifyTexture(GLuint TexID)
{
	glBindTexture(GL_TEXTURE_2D, TexID);
	glutMainLoopEvent();
	return GL_TRUE;
}
