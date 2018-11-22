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

	//�޸Ĵ˲������Զ��˶��ٶȽ����޸�
	GLdouble scale = 2.0;

	//����Ϊ��������ķ�������
	GLdouble directionX=0;
	GLdouble directionZ=1;

	//staticMove ����ʹ��
	GLdouble lastChangeDirPosX=-100;
	GLdouble lastChangeDirPosZ=-100;

	//����Ϊ��һ�β�����Ҫת���Ľ�
	float rotateAngle = 0.0;

	Model* innerObject=nullptr;

public:
	void setPos(GLdouble _posX, GLdouble _posY, GLdouble _posZ) {
		posX = _posX;
		posY = _posY;
		posZ = _posZ;
	}
	//���������ĳ���(directionX,directionZ)Ӧ����һ����λ����
	//�����(0,0)���˵�ǰ��λ��Ϊ���
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
	//ģ�����ڵ�������߶�����Ϊ��ֻ�޸�����ĵ�ǰλ��
	void randomMove();
	//Ĭ��yֵ�����߶Ȳ��䣩,��һ����Χ������߶�
	void randomMoveWithLimit(GLdouble posX1, GLdouble posZ1, GLdouble posX2, GLdouble posZ2);
	//����ͣͣ�ߵ�posX��posY
	void randomMoveToPoint(GLdouble posX, GLdouble posZ);
	//��ʾ�ú��� 
	void staticMove();
	
	~Human() {
		if (this->innerObject != nullptr) {
			delete innerObject;
		}
	}
};
