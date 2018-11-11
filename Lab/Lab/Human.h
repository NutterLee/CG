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

	//�޸Ĵ˲������Զ��˶��ٶȽ����޸�
	GLdouble scale = 1.0;

	Model* innerObject=nullptr;

public:
	void draw(Shader shader);
	void loadModel(string path);
	//ģ�����ڵ�������߶�����Ϊ��ֻ�޸�����ĵ�ǰλ��
	void randomMove();
	//Ĭ��yֵ�����߶Ȳ��䣩,��һ����Χ������߶�
	void randomMoveWithLimit(GLdouble posX1, GLdouble posZ1, GLdouble posX2, GLdouble posZ2);
	~Human() {
		if (this->innerObject != nullptr) {
			delete innerObject;
		}
	}
};
