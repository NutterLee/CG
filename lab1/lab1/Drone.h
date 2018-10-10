#pragma once
#include <iostream>
#include "ObjLoader.h"
#include <random>
#include <cmath>

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
	GLdouble maxSpeedX=1;
	GLdouble maxSpeedY=1;
	GLdouble maxSpeedZ=1;	
	//每一帧三维上drone的最大的加速度变化值（绝对值）
	GLdouble maxAccX = 0.2;
	GLdouble maxAccY = 0.2;
	GLdouble maxAccZ = 0.2;
	//每一帧的加速度的值
	GLdouble accX = 0;
	GLdouble accY = 0;
	GLdouble accZ = 0;
	//drone在三维上的速度
	GLdouble speedX;
	GLdouble speedY;
	GLdouble speedZ;
	//上一次加速度变化点的位移
	GLdouble lastChangeAccPosX=999;
	GLdouble lastChangeAccPosY=999;
	GLdouble lastChangeAccPosZ=999;
	//距离悬停的点的最大允许偏移量
	GLdouble maxHoverLength = 5.0;
	// 飞行mode,state 0表示非攻击mode，1表示攻击mode
	int mode = 0;
	//攻击mode下发动快速靠近的最大距离
	GLdouble maxAttackLength = 10.0;
	ObjLoader innerObject;
	GLdouble moveScale = 100.0;
	//最大允许的接近误差
	GLdouble maxStopLength = 0.2;
	//每一帧的时间间隔
	GLdouble timeSlice = 20/100.0;

	//测试使用的参数
	bool escapeFinish = true;
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
		maxSpeedX = _maxX;
		maxSpeedY = _maxY;
		maxSpeedZ = _maxZ;
	}

	void setLastChangeAccPos(GLdouble _lastX, GLdouble _lastY, GLdouble _lastZ) {
		lastChangeAccPosX = _lastX;
		lastChangeAccPosY = _lastY;
		lastChangeAccPosZ = _lastZ;
	}

	//下面是每一帧都被调用的函数
	//辅助函数，用于判断是否需要修改acc
	//判断条件：某个方向上的位移量超过了最大允许误差的1/2
	bool needChangeAcc() {
		GLdouble scale = maxStopLength * 5;
		return (abs(posX - lastChangeAccPosX) >scale || abs(posY - lastChangeAccPosY) >scale || abs(posZ - lastChangeAccPosZ) > scale
			||accX==0||accY==0||accZ==0);
		//return true;
	}

	//更新修改acc的位置记录为当前位置
	void updateLastChangeAccPos() {
		lastChangeAccPosX = posX;
		lastChangeAccPosY = posY;
		lastChangeAccPosZ = posZ;
	}

	//生成随机的加速度，注意只有在位置偏移超过一定程度（最大允许误差的一半）才会再次生成
	//之所以这样设计是为了使得随机位移更加明显
	void changeAcc();

	//生成带有方向指向性的加速度，这个加速度和参数向量的方向会有少量偏差，但是总体角度不会变化
	void changeAccWithTargeDirect(GLdouble verX, GLdouble verY, GLdouble verZ);

	//生成带有区域限制性的加速度，这个加速度的大小不确定，但如果某个方向坐标已经离边界的距离少于允许最大偏移的一半，则将该方向的加速度指向和运动速度的反向
	//space的边界由两个对角线构成的立方体决定
	void changeAccWithLimitSpace(GLdouble posX1, GLdouble posY1, GLdouble posZ1, GLdouble posX2, GLdouble posY2, GLdouble posZ2);

	//生成目标为target的加速度
	void changeAccWithTargePos(GLdouble _tarX, GLdouble _tarY, GLdouble _tarZ);

	//根据加速度来变化速度
	void changeSpeed() {
		//各个方向的速度都不应该超过max限制
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

	//根据速度来变化坐标
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