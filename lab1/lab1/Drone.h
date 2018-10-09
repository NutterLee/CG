#pragma once
#include <iostream>
#include "ObjLoader.h"
#include <random>

class Drone {
private:
	//��ǰ����
	GLdouble posX;
	GLdouble posY;
	GLdouble posZ;
	//Ŀ������
	GLdouble targetX;
	GLdouble targetY;
	GLdouble targetZ;
	//ÿһ֡��drone����ά������λ�ƣ��ٶ����ƣ�
	GLdouble maxPerX;
	GLdouble maxPerY;
	GLdouble maxPerZ;
	//�������忿��������ٶ�
	GLdouble maxApproachSpeed;
	// ����mode,state 0��ʾ�ǹ���mode��1��ʾ����mode
	int mode = 0;
public:
	//����drone��obj
	void load(string path);
	//�ɽ�Ŀ�����꣬��ȡ��Ӧ��mode
	void flyToPos(GLdouble tarX, GLdouble tarY, GLdouble tarZ, int mode);
	//��ͣ��ĳ������
	void hoverAtPos(GLdouble tarX, GLdouble tarY, GLdouble tarZ);
	//��������ĳ������Ĺ���������Ϊ���������������������꣬ʱ����1����2
	void escapeFromPos(GLdouble posX1, GLdouble posY1, GLdouble posZ1, GLdouble posX2, GLdouble posY2, GLdouble posZ2);
	//��ĳ����Χ����ѰĿ�꣬��ΧΪ����������Ϊ�Խ��ߵĿռ�����
	void searchInArea(GLdouble posX1,GLdouble posY1,GLdouble posZ1,)
};