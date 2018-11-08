#pragma once
#include "GL\glew.h"
#include "GL\freeglut.h"
#include"GL\glut.h"
class MarkSphere {
private:
	GLdouble posX;
	GLdouble posY;
	GLdouble posZ;
	double radius;

public:
	void draw() {
		glTranslated(posX, posY, posZ);
		glutSolidSphere(radius, 16, 16);
	}
	void setPos(GLdouble _posX, GLdouble _posY, GLdouble _posZ) {
		posX = _posX;
		posY = _posY;
		posZ = _posZ;
	}
	void setRadius(double _radius) {
		radius = _radius;
	}
};