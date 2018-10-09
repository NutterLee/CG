#pragma once
#include <iostream>
#include "ObjLoader.h"
#include <random>

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
	GLdouble maxPerX=0.1;
	GLdouble maxPerY=0.1;
	GLdouble maxPerZ=0.1;	
	// ����mode,state 0��ʾ�ǹ���mode��1��ʾ����mode
	int mode = 0;
	//����mode�·������ٿ�����������
	GLdouble maxAttackLength = 10.0;
	ObjLoader innerObject;
	GLdouble moveScale = 100.0;
	//�������Ľӽ����
	GLdouble maxStopLength = 0.2;
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
		maxPerX = _maxX;
		maxPerY = _maxY;
		maxPerZ = _maxZ;
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