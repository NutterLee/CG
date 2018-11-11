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

	GLdouble maxAcc = 0.05;

	//修改此参数可以对运动速度进行修改
	GLdouble scale = 1.0;

	Model* innerObject=nullptr;

public:
	void draw(Shader shader);
	void loadModel(string path);
	//模仿人在地面随机走动的行为，只修改人物的当前位置
	void randomMove();
	//默认y值（即高度不变）,在一定范围内随机走动
	void randomMoveWithLimit(GLdouble posX1, GLdouble posZ1, GLdouble posX2, GLdouble posZ2);
	~Human() {
		if (this->innerObject != nullptr) {
			delete innerObject;
		}
	}
};
