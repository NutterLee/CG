#pragma once
#include<cmath>
class Camera
{
private:
	double theta;      //ȷ��x��z��λ��
	double y;          //yλ��
	double dTheta;     //�Ƕ�����
	double dy;         //����y����
public:
	//�๹�캯����Ĭ�ϳ�ʼ���÷�
	Camera() : theta(0), y(120), dTheta(0.04), dy(0.2) {}
	//�෽��
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
