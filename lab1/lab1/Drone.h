#pragma once
#include <iostream>
#include "ObjLoader.h"
#include <random>
#include <cmath>

class Drone {
private:
	//��ǰ����
	GLdouble posX=0.0;
	GLdouble posY=0.0;
	GLdouble posZ=0.0;
	//Ŀ������
	GLdouble targetX;
	GLdouble targetY;
	GLdouble targetZ;
	//fly2���õ���ʱ����
	GLdouble tmpTargetX;
	GLdouble tmpTargetY;
	GLdouble tmpTargetZ;
	//ÿһ֡��drone����ά������λ�ƣ��ٶ����ƣ�
	GLdouble maxSpeedX=1;
	GLdouble maxSpeedY=1;
	GLdouble maxSpeedZ=1;	
	//ÿһ֡��ά��drone�����ļ��ٶȱ仯ֵ������ֵ��
	GLdouble maxAccX = 0.2;
	GLdouble maxAccY = 0.2;
	GLdouble maxAccZ = 0.2;
	//ÿһ֡�ļ��ٶȵ�ֵ
	GLdouble accX = 0;
	GLdouble accY = 0;
	GLdouble accZ = 0;
	//drone����ά�ϵ��ٶ�
	GLdouble speedX;
	GLdouble speedY;
	GLdouble speedZ;
	//��һ�μ��ٶȱ仯���λ��
	GLdouble lastChangeAccPosX=999;
	GLdouble lastChangeAccPosY=999;
	GLdouble lastChangeAccPosZ=999;
	//������ͣ�ĵ���������ƫ����
	GLdouble maxHoverLength = 5.0;
	// ����mode,state 0��ʾ�ǹ���mode��1��ʾ����mode
	int mode = 0;
	//����mode�·������ٿ�����������
	GLdouble maxAttackLength = 10.0;
	ObjLoader innerObject;
	GLdouble moveScale = 100.0;
	//�������Ľӽ����
	GLdouble maxStopLength = 0.2;
	//ÿһ֡��ʱ����
	GLdouble timeSlice = 20/100.0;

	//����ʹ�õĲ���
	bool escapeFinish = true;
public:
	//ÿһ֡�����е�λ����
	GLdouble toMoveX = 0.0;
	GLdouble toMoveY = 0.0;
	GLdouble toMoveZ = 0.0;
	//����drone��obj
	void load(string path);	
	Drone(GLdouble _posX=0, GLdouble _posY=0, GLdouble _posZ=0) {
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
		return (abs(posX - lastChangeAccPosX) >scale || abs(posY - lastChangeAccPosY) >scale || abs(posZ - lastChangeAccPosZ) > scale
			||accX==0||accY==0||accZ==0);
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

	//����Ŀ��Ϊtarget�ļ��ٶ�
	void changeAccWithTargePos(GLdouble _tarX, GLdouble _tarY, GLdouble _tarZ);

	//���ݼ��ٶ����仯�ٶ�
	void changeSpeed() {
		//����������ٶȶ���Ӧ�ó���max����
		speedX =speedX + accX*timeSlice;
		if (abs(speedX) > maxSpeedX) {
			speedX = speedX - 2 * accX*timeSlice;
		}
		speedY = speedY + accY*timeSlice;
		if (abs(speedY) > maxSpeedY) {
			speedY = speedY - 2 * accY*timeSlice;
		}
		speedZ = speedZ +  accZ*timeSlice;
		if (abs(speedZ) > maxSpeedZ) {
			speedZ = speedZ - 2 * accZ*timeSlice;
		}
		cout << "current speed: " << speedX << "," << speedY << "," << speedZ << endl;
	}

	//�����ٶ����仯����
	void changePos() {
		posX += accX*timeSlice+speedX;
		posY += accY*timeSlice+speedY;
		posZ += accZ*timeSlice+speedZ;
	}
	

	void setMaxAcc(GLdouble _accX, GLdouble _accY, GLdouble _accZ) {
		maxAccX = _accX;
		maxAccY = _accY;
		maxAccZ = _accZ;
	}
	//���µĺ������Ƕ�λ���������޸ģ�����������draw��λ���� ���ֳ���
	//�ɽ�Ŀ�����꣬��ȡ��Ӧ��mode
	void flyToPos(GLdouble tarX, GLdouble tarY, GLdouble tarZ, int mode);
	//�ɽ�Ŀ��
	void flyToPos2(GLdouble tarX, GLdouble tarY, GLdouble tarZ, int mode);
	//��ͣ��ĳ������
	void hoverAtPos(GLdouble tarX, GLdouble tarY, GLdouble tarZ);
	//��������ĳ������Ĺ���������Ϊ���������������������꣬ʱ����1����2
	void escapeFromPos(GLdouble posX1, GLdouble posY1, GLdouble posZ1, GLdouble posX2, GLdouble posY2, GLdouble posZ2);
	//��ĳ����Χ����ѰĿ�꣬��ΧΪ����������Ϊ�Խ��ߵĿռ�����
	void searchInArea(GLdouble posX1, GLdouble posY1, GLdouble posZ1, GLdouble posX2, GLdouble posY2, GLdouble posZ2);	
	//��ͼ����������ƫ������ͼ
	//ͬʱ��������
	void draw();

	GLdouble getPosX() { return posX; }
	GLdouble getPosY() { return posY; }
	GLdouble getPosZ() { return posZ; }
};