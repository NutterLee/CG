#pragma once
#include "Model.h"

class  Mortar
{
public:
	void setPos(GLdouble _posX, GLdouble _posY, GLdouble _posZ) {
		posX = _posX;
		posY = _posY;
		posZ = _posZ;
	}
	GLdouble getPosX() {
		return posX;
	}
	GLdouble getPosY() {
		return posY;
	}
	GLdouble getPosZ() {
		return posZ;
	}

	void setRotateAngle(float angle) {
		rotateAngle = angle;
	}
	float getRotateAngle() {
		return rotateAngle;
	}

	//将固定位置的迫击炮转到某个地步
	void rotateToTarget(GLdouble _posX, GLdouble _posY, GLdouble _posZ);
	void draw(Shader shader);
	void loadModel(string path);
	~Mortar() {
		if (innerObject != nullptr)
			delete innerObject;
		return;
	}

private:
	Model* innerObject;
	GLdouble posX;
	GLdouble posY;
	GLdouble posZ;
	float rotateAngle;

};

 