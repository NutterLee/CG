#pragma once
#include "Model.h"
#include <random>
#include <cmath>

class Human {
private:
	GLdouble posX;
	GLdouble posY;
	GLdouble posZ;

	GLdouble accX;
	//GLdouble accY;
	GLdouble accZ;

	GLdouble speedX;
	//GLdouble speedY;
	GLdouble speedZ;

	GLdouble maxAcc = 0.3;

	//修改此参数可以对运动速度进行修改
	GLdouble scale = 2.0;

	//以下为人脸朝向的方向向量
	GLdouble directionX=0;
	GLdouble directionZ=1;

	//staticMove 函数使用
	GLdouble lastChangeDirPosX=-100;
	GLdouble lastChangeDirPosZ=-100;

	//以下为下一次操作需要转动的角
	float rotateAngle = 0.0;

	Model* innerObject=nullptr;

public:
	void setPos(GLdouble _posX, GLdouble _posY, GLdouble _posZ) {
		posX = _posX;
		posY = _posY;
		posZ = _posZ;
	}
	//设置人脸的朝向，(directionX,directionZ)应该是一个单位向量
	//起点是(0,0)以人当前的位置为起点
	void setDirection(GLdouble _dirX, GLdouble _dirZ) {
		directionX = _dirX;
		directionZ = _dirZ; 
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
	float getRotateAngle() {
		return rotateAngle;
	}
	void setRotateAngle(GLdouble _ang) {
		rotateAngle = _ang;
	}

	void draw(Shader shader);
	void loadModel(string path);
	//模仿人在地面随机走动的行为，只修改人物的当前位置
	void randomMove();
	//默认y值（即高度不变）,在一定范围内随机走动
	void randomMoveWithLimit(GLdouble posX1, GLdouble posZ1, GLdouble posX2, GLdouble posZ2);
	//走走停停走到posX，posY
	void randomMoveToPoint(GLdouble posX, GLdouble posZ);
	//演示用函数 
	void staticMove();
	
	~Human() {
		if (this->innerObject != nullptr) {
			delete innerObject;
		}
	}
};
