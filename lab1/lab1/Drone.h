#pragma once
#include <iostream>
#include "ObjLoader.h"
#include <random>

class Drone {
private:
	//当前坐标
	GLdouble posX;
	GLdouble posY;
	GLdouble posZ;
	//目标坐标
	GLdouble targetX;
	GLdouble targetY;
	GLdouble targetZ;
	//每一帧中drone在三维上最大的位移（速度限制）
	GLdouble maxPerX;
	GLdouble maxPerY;
	GLdouble maxPerZ;
	//允许物体靠近的最大速度
	GLdouble maxApproachSpeed;
	// 飞行mode,state 0表示非攻击mode，1表示攻击mode
	int mode = 0;
public:
	//读入drone的obj
	void load(string path);
	//飞近目标坐标，采取对应的mode
	void flyToPos(GLdouble tarX, GLdouble tarY, GLdouble tarZ, int mode);
	//悬停在某个坐标
	void hoverAtPos(GLdouble tarX, GLdouble tarY, GLdouble tarZ);
	//闪避来自某个方向的攻击，参数为攻击方向的物体的两次坐标，时间上1早于2
	void escapeFromPos(GLdouble posX1, GLdouble posY1, GLdouble posZ1, GLdouble posX2, GLdouble posY2, GLdouble posZ2);
	//在某个范围内搜寻目标，范围为由两个坐标为对角线的空间区域
	void searchInArea(GLdouble posX1,GLdouble posY1,GLdouble posZ1,)
};