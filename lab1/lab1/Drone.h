#pragma once
#include <iostream>
#include "ObjLoader.h"
#include <random>

class Drone {
private:
	//当前坐标
	GLdouble posX=0.0;
	GLdouble posY=0.0;
	GLdouble posZ=0.0;
	//目标坐标
	GLdouble targetX;
	GLdouble targetY;
	GLdouble targetZ;
	//fly2采用的临时变量
	GLdouble tmpTargetX;
	GLdouble tmpTargetY;
	GLdouble tmpTargetZ;
	//每一帧中drone在三维上最大的位移（速度限制）
	GLdouble maxPerX=0.1;
	GLdouble maxPerY=0.1;
	GLdouble maxPerZ=0.1;	
	// 飞行mode,state 0表示非攻击mode，1表示攻击mode
	int mode = 0;
	//攻击mode下发动快速靠近的最大距离
	GLdouble maxAttackLength = 10.0;
	ObjLoader innerObject;
	GLdouble moveScale = 100.0;
	//最大允许的接近误差
	GLdouble maxStopLength = 0.2;
public:
	//每一帧将进行的位移量
	GLdouble toMoveX = 0.0;
	GLdouble toMoveY = 0.0;
	GLdouble toMoveZ = 0.0;

	//读入drone的obj
	void load(string path);	
	Drone(GLdouble _posX=0, GLdouble _posY=0, GLdouble _posZ=0) {
		setPos(_posX, _posY, _posZ);
	}
	//设置基本参数
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
	//以下的函数都是对位移量进行修改，再在最后调用draw将位移量 体现出来
	//飞近目标坐标，采取对应的mode
	void flyToPos(GLdouble tarX, GLdouble tarY, GLdouble tarZ, int mode);
	//飞近目标
	void flyToPos2(GLdouble tarX, GLdouble tarY, GLdouble tarZ, int mode);
	//悬停在某个坐标
	void hoverAtPos(GLdouble tarX, GLdouble tarY, GLdouble tarZ);
	//闪避来自某个方向的攻击，参数为攻击方向的物体的两次坐标，时间上1早于2
	void escapeFromPos(GLdouble posX1, GLdouble posY1, GLdouble posZ1, GLdouble posX2, GLdouble posY2, GLdouble posZ2);
	//在某个范围内搜寻目标，范围为由两个坐标为对角线的空间区域
	void searchInArea(GLdouble posX1, GLdouble posY1, GLdouble posZ1, GLdouble posX2, GLdouble posY2, GLdouble posZ2);	
	//绘图函数，根据偏移量绘图
	//同时更新坐标
	void draw();

	GLdouble getPosX() { return posX; }
	GLdouble getPosY() { return posY; }
	GLdouble getPosZ() { return posZ; }
};