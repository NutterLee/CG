#pragma once
#include "ObjLoader.h"

//环境的模块，将整个环境直接载入

class Env {
private:
	GLdouble posX;
	GLdouble posY;
	GLdouble posZ;
	ObjLoader innerObject;

public:
	void setPos(GLdouble _posX, GLdouble _posY, GLdouble _posZ) {
		posX = _posX;
		posY = _posY;
		posZ = _posZ;
	}
	void load(string path) {
		innerObject = ObjLoader(path);
	}
	void draw() {
		glTranslatef(this->posX, this->posY, this->posZ);
		innerObject.Draw();
	}
};