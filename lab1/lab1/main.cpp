#include "ObjLoader.h"
#include <random>
#include<iostream>
using namespace std;
//模型路径
string filePath = "F:\\CG\\lab1\\data\\drone\\Drone.obj";


//飞行器的坐标
GLdouble posX = 0;
GLdouble posY = 0;
GLdouble posZ = 0;
//随机的偏移
GLdouble deltaX = 0;
GLdouble deltaY = 0;
GLdouble deltaZ = 0;
//每一次要运动的偏移
GLdouble toMoveX = 0;
GLdouble toMoveY = 0;
GLdouble toMoveZ = 0;
//目标所在的位置
GLdouble targetX = 100;
GLdouble targetY = 100;
GLdouble targetZ = 100;
//每次渲染最大的步长
GLdouble maxPerX = 0.1;
GLdouble maxPerY = 0.1;
GLdouble maxPerZ = 0.1;
std::default_random_engine generator;
std::uniform_int_distribution<int> dis(-2, 2);
ObjLoader objModel = ObjLoader(filePath);
//实现移动鼠标观察模型所需变量
static float c = 3.1415926 / 180.0f;
static float r = 60.0f;
static int degree = 90;
static int oldPosY = -1;
static int oldPosX = -1;

//安置光源
void setLightRes() {
	GLfloat lightPosition[] = { 0.0f, 0.0f, 1.0f, 0.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
	glEnable(GL_LIGHTING); //启用光源
	glEnable(GL_LIGHT0);   //使用指定灯光
}

//初始化
void init() {
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutCreateWindow("ObjLoader");
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
	setLightRes();
	glEnable(GL_DEPTH_TEST);
	gluLookAt(r*cos(c*degree), 0, r*sin(c*degree), 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
}

void display()
{
	glColor3f(1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity(); 
	//更新位置记录信息
	posX += toMoveX;
	posY += toMoveY;
	posZ += toMoveZ;
	glTranslatef(toMoveX, toMoveY, toMoveZ);
	cout << "current pos:" << posX << ", " << posY << ", " << posZ << endl;
	setLightRes();
	glPushMatrix();
	objModel.Draw();//绘制obj模型
	glPopMatrix();
	glutSwapBuffers();
}

//time function作用：
//1. 生成随机偏移量
//2. 将drone向target靠拢
void TimeFunction(int value)
{
	deltaX = dis(generator)/100.0;
	deltaY = dis(generator)/100.0;
	deltaZ = dis(generator)/100.0;
	GLdouble leftX = targetX - posX;
	GLdouble leftY = targetY - posY;
	GLdouble leftZ = targetZ - posZ;
	GLdouble staticMoveX=0;
	GLdouble staticMoveY=0;
	GLdouble staticMoveZ=0;
	if (leftX > 0) {
		staticMoveX = maxPerX > leftX ? leftX : maxPerX;
	}
	else if (leftX < 0) {
		staticMoveX = -maxPerX > leftX ? -maxPerX : leftX;
	}
	if (leftY > 0) {
		staticMoveY = maxPerY > leftY ? leftY : maxPerY;
	}
	else if (leftY < 0) {
		staticMoveY = -maxPerY > leftY ? -maxPerY : leftY;
	}
	if (leftZ > 0) {
		staticMoveZ = maxPerZ > leftZ ? leftZ : maxPerZ;
	}
	else if (leftZ < 0) {
		staticMoveZ = -maxPerZ > leftZ? -maxPerZ : leftZ;
	}
	toMoveX = staticMoveX + deltaX;
	toMoveY = staticMoveY + deltaY;
	toMoveZ = staticMoveZ + deltaZ;
	if (leftX == 0) {
		toMoveX = 0;
	}
	if (leftY == 0) {
		toMoveY = 0;
	}
	if (leftZ == 0) {
		toMoveZ = 0;
	}	
	glutTimerFunc(20, TimeFunction, 1);
}

void reshape(int width, int height)
{
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0f, (GLdouble)width / (GLdouble)height, 1.0f, 200.0f);
	glMatrixMode(GL_MODELVIEW);
}

//移动鼠标360观察模型
void moseMove(int button, int state, int x, int y)
{
	if (state == GLUT_DOWN) {
		oldPosX = x; oldPosY = y;
	}
}
void changeViewPoint(int x, int y)
{
	int temp = x - oldPosX;
	degree += temp;
	oldPosX = x;
	oldPosY = y;
}

void myIdle()
{
	glutPostRedisplay();
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMouseFunc(moseMove);
	glutMotionFunc(changeViewPoint);
	glutIdleFunc(myIdle);
	glutTimerFunc(20 , TimeFunction, 1);
	glutMainLoop();
	return 0;
}
