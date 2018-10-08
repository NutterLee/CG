#include "GL\glew.h"
#include "GL\freeglut.h"
#include "GL/glut.h"
#include<iostream>
using namespace std;

int main(int  args, char ** argv)
{
	glutInit(&args, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutCreateWindow("Hello openGL");
	cout << "OpenGL°æ±¾ºÅ" << glGetString(GL_VERSION) << endl;
	glutMainLoop();
	return 0;
}