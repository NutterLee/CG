#pragma once
#include<cmath>
class Camera
{
private:
	double theta;      //确定x和z的位置
	double y;          //y位置
	double dTheta;     //角度增量
	double dy;         //上下y增量
public:
	//类构造函数—默认初始化用法
	Camera() : theta(0), y(120), dTheta(0.04), dy(0.2) {}
	//类方法
	double getX() { return 100 * cos(theta); }
	double getY() { return y; }
	double getZ() { return 100 * sin(theta); }
	double getTheta() { return theta; }
	void setTheta(double newT) { theta = newT; }
	void setY(double newY) { y = newY; }
	void moveRight() { theta += dTheta; }
	void moveLeft() { theta -= dTheta; }
	void moveUp() { y += dy; }
	void moveDown() { if (y > dy) y -= dy; }
};
