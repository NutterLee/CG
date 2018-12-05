#pragma once
#include <iostream>
#include "Model.h"
#include "Camera.h"
#include <random>
#include <cmath>
// GLM Mathemtics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Drone {
private:
	//��ǰ����
	GLdouble posX = 0.0;
	GLdouble posY = 0.0;
	GLdouble posZ = 0.0;
	//Ŀ������
	GLdouble targetX;
	GLdouble targetY;
	GLdouble targetZ;
	//ÿһ֡��drone����ά�������ٶ�
	GLdouble maxSpeedX = 0.01;
	GLdouble maxSpeedY = 0.01;
	GLdouble maxSpeedZ = 0.01;
	//ÿһ֡��ά��drone�����ļ��ٶȱ仯ֵ������ֵ��
	GLdouble maxAccX = 0.003;
	GLdouble maxAccY = 0.003;
	GLdouble maxAccZ = 0.003;
	//ÿһ֡�ļ��ٶȵ�ֵ
	GLdouble accX = 0.005;
	GLdouble accY = 0.005;
	GLdouble accZ = 0.005;
	//drone����ά�ϵ��ٶ�
	GLdouble speedX;
	GLdouble speedY;
	GLdouble speedZ;
	//��һ�μ��ٶȱ仯���λ��
	GLdouble lastChangeAccPosX = 999;
	GLdouble lastChangeAccPosY = 999;
	GLdouble lastChangeAccPosZ = 999;
	//������ͣ�ĵ���������ƫ����
	GLdouble maxHoverLength = 0.50;
	// ����mode,state 0��ʾ�ǹ���mode��1��ʾ����mode
	int mode = 0;
	//����mode�·������ٿ�����������
	GLdouble maxAttackLength = 1.00;

	//�������Χ
	GLdouble maxDetectLengrh = 2.50;

	Model* innerObject=nullptr;
	GLdouble moveScale = 100.0;
	//�������Ľӽ����
	GLdouble maxStopLength = 0.2;
	//ÿһ֡��ʱ����
	GLdouble timeSlice = 20 / 100.0;

	//����ʹ�õĲ���
	bool escapeFinish = true;
	bool hasEscaped = false;

public:
	//����/״̬��ԭ����
	void reset();
	//����drone��obj
	void loadModel(string path);
	GLdouble getMaxDetectLength()
	{
		return maxDetectLengrh;
	}

	~Drone() {
		if(innerObject!=nullptr)
			delete this->innerObject;
	}

	Drone(GLdouble _posX = 0, GLdouble _posY = 0, GLdouble _posZ = 0) {
		setPos(_posX, _posY, _posZ);
	}
	//���û�������
	void setPos(GLdouble _posX, GLdouble _posY, GLdouble _posZ) {
		posX = _posX;
		posY = _posY;
		posZ = _posZ;
	}
	void setSpeed(GLdouble _maxX, GLdouble _maxY, GLdouble _maxZ) {
		maxSpeedX = _maxX;
		maxSpeedY = _maxY;
		maxSpeedZ = _maxZ;
	}

	void setLastChangeAccPos(GLdouble _lastX, GLdouble _lastY, GLdouble _lastZ) {
		lastChangeAccPosX = _lastX;
		lastChangeAccPosY = _lastY;
		lastChangeAccPosZ = _lastZ;
	}

	//������ÿһ֡�������õĺ���
	//���������������ж��Ƿ���Ҫ�޸�acc
	//�ж�������ĳ�������ϵ�λ���������������������1/2
	bool needChangeAcc() {
		GLdouble scale = maxStopLength * 5;
		GLdouble xChange = abs(posX - lastChangeAccPosX);
		GLdouble yChange = abs(posY - lastChangeAccPosY);
		GLdouble zChange = abs(posZ - lastChangeAccPosZ);
		//cout << "scale:" << scale << ", changes:" << xChange << "," << yChange << "," << zChange << endl;
		return xChange >= scale || yChange >= scale || zChange >= scale;
		//return true;
	}

	//�����޸�acc��λ�ü�¼Ϊ��ǰλ��
	void updateLastChangeAccPos() {
		lastChangeAccPosX = posX;
		lastChangeAccPosY = posY;
		lastChangeAccPosZ = posZ;
	}

	//��������ļ��ٶȣ�ע��ֻ����λ��ƫ�Ƴ���һ���̶ȣ������������һ�룩�Ż��ٴ�����
	//֮�������������Ϊ��ʹ�����λ�Ƹ�������
	void changeAcc();

	//���ɴ��з���ָ���Եļ��ٶȣ�������ٶȺͲ��������ķ����������ƫ���������ǶȲ���仯
	void changeAccWithTargeDirect(GLdouble verX, GLdouble verY, GLdouble verZ);

	//���ɴ������������Եļ��ٶȣ�������ٶȵĴ�С��ȷ���������ĳ�����������Ѿ���߽�ľ��������������ƫ�Ƶ�һ�룬�򽫸÷���ļ��ٶ�ָ����˶��ٶȵķ���
	//space�ı߽��������Խ��߹��ɵ����������
	void changeAccWithLimitSpace(GLdouble posX1, GLdouble posY1, GLdouble posZ1, GLdouble posX2, GLdouble posY2, GLdouble posZ2);

	//���ݼ��ٶ����仯�ٶ�
	void changeSpeed() {
		//����������ٶȶ���Ӧ�ó���max����
		speedX = speedX + accX*timeSlice;
		if (abs(speedX) > maxSpeedX) {
			speedX = speedX - 2 * accX*timeSlice;
		}
		speedY = speedY + accY*timeSlice;
		if (abs(speedY) > maxSpeedY) {
			speedY = speedY - 2 * accY*timeSlice;
		}
		speedZ = speedZ + accZ*timeSlice;
		if (abs(speedZ) > maxSpeedZ) {
			speedZ = speedZ - 2 * accZ*timeSlice;
		}
		//cout << "current speed: " << speedX << "," << speedY << "," << speedZ << endl;
	}

	//�����ٶ����仯����
	void changePos() {
		posX += accX*timeSlice + speedX;
		posY += accY*timeSlice + speedY;
		posZ += accZ*timeSlice + speedZ;
		//cout << "drone pos: " << posX << ", " << posY << ", " << posZ << endl;
	}

	void setMaxAcc(GLdouble _accX, GLdouble _accY, GLdouble _accZ) {
		maxAccX = _accX;
		maxAccY = _accY;
		maxAccZ = _accZ;
	}
	//���µĺ������Ƕ�λ���������޸ģ�����������draw��λ���� ���ֳ���
	//���ٿ���Ŀ��
	void flyToPos(GLdouble tarX, GLdouble tarY, GLdouble tarZ, int mode);
	//�ɽ�Ŀ��
	void flyToPos2(GLdouble tarX, GLdouble tarY, GLdouble tarZ, int mode);
	//��ͣ��ĳ������
	void hoverAtPos(GLdouble tarX, GLdouble tarY, GLdouble tarZ);
	//��������ĳ������Ĺ���������Ϊ���������������������꣬ʱ����1����2
	void escapeFromPos(GLdouble posX1, GLdouble posY1, GLdouble posZ1, GLdouble posX2, GLdouble posY2, GLdouble posZ2);
	//��ĳ����Χ����ѰĿ�꣬��ΧΪ����������Ϊ�Խ��ߵĿռ�����
	void searchInArea(GLdouble posX1, GLdouble posY1, GLdouble posZ1, GLdouble posX2, GLdouble posY2, GLdouble posZ2);
	//׹�亯��
	void falldown();
	//��ͼ����
	void draw(Shader shader);

	GLdouble getPosX() { return posX; }
	GLdouble getPosY() { return posY; }
	GLdouble getPosZ() { return posZ; }

	//�ж����˻��Ƿ���������ײ�ĺ���
	//��ΪҪ����һЩƫ������������ʵҪ�����˻���λ�ý���һЩ����
	bool hasFound(GLdouble posX, GLdouble posY, GLdouble posZ);

	
};

GLdouble min(GLdouble x, GLdouble y);

GLdouble max(GLdouble x, GLdouble y);