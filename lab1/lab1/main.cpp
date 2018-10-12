#include "ObjLoader.h"
#include"Drone.h"
#include"BaseFloor.h"
#include <random>
#include<iostream>
#include"Camera.h"
#include"MarkSphere.h"
#include <stdlib.h>
using namespace std;
//模型路径
string filePath = "..\\data\\drone\\Drone.obj";
Drone drone;
BaseFloor baseFloor(300, 300);
Camera camera;
MarkSphere startSphere;
MarkSphere stopSphere;
//实现移动鼠标观察模型所需变量
static int oldPosY = -1;
static int oldPosX = -1;
//drone飞行的目的地
GLdouble targetX=0;
GLdouble targetY=0;
GLdouble targetZ=0;
//测试时用的模式
int displayMode = 0;

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
	glutInitWindowSize(700, 700);
	glutCreateWindow("ObjLoader");
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
	setLightRes();
	glEnable(GL_LIGHT1);
	glEnable(GL_DEPTH_TEST);	
	baseFloor.create();	
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity(); 	
	setLightRes();	
	gluLookAt(camera.getX(), camera.getY(), camera.getZ(),    //摄像机位置
		baseFloor.centerX(), 0.0, baseFloor.centerZ(),   //焦点坐标
		0.0, 1.0, 0.0);   //摄像机机顶方向矢量
	baseFloor.draw();
	startSphere.draw();
	stopSphere.draw();
	drone.draw();
	glutSwapBuffers();
}

//time function作用：
//1. 生成随机偏移量
//2. 将drone向target靠拢
void TimeFunction(int value)
{	
	switch (displayMode)
	{
	case 0: {
		drone.reset();
		drone.setPos(baseFloor.centerX()*0.3, 10, baseFloor.centerZ()*0.5);
		break;
	}
	case 1: {
		drone.flyToPos(0, 0, 0, 0);
		break;
	}
	case 2: {
		drone.flyToPos2(0.0, 0.0, 0.0, 0);
		break;
	}
	case 3: {
		drone.hoverAtPos(baseFloor.centerX()*0.3, 10, baseFloor.centerZ()*0.5);
		break;
	}
	case 4: {
		drone.escapeFromPos(0, 0, 0, 1, 1, 1);
		break;
	}
	case 5: {
		drone.searchInArea(0, 10, 0, 100, 40, 100);
		break;
	}
	default:
		break;
	}
	glutTimerFunc(20, TimeFunction, 1);
}

void handleKeybord(unsigned char key, int x, int y) {
	//按esc时结束进程
	cout << "got key:" << key << endl;
	drone.reset();
	drone.setPos(baseFloor.centerX()*0.3, 10, baseFloor.centerZ()*0.5);
	switch (key)
	{
	case 'a': {
		exit(0);
	}
	case 's': {
		displayMode = 0;
		break;
	}
	case 'd': {
		displayMode = 1;
		break;
	}
	case 'f': {
		displayMode = 2;
		break;
	}
	case 'g': {
		displayMode = 3;
		break;
	}
	case 'h': {
		displayMode = 4;
		break;
	}
	case 'j': {
		displayMode = 5;
		break;
	}
	default:
		break;
	}
}

void reshape(int width, int height)
{
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(80.0f, (GLdouble)width / (GLdouble)height, 1.0f, 700.0f);
	glMatrixMode(GL_MODELVIEW);
}

//移动鼠标360观察模型
void mouseMove(int button, int state, int x, int y)
{
	if (state == GLUT_DOWN) {
		oldPosX = x; oldPosY = y;
	}
}
void changeViewPoint(int x, int y)
{
	//set theta change
	double temp_gapX = ((double)x - (double)oldPosX)*0.05;
	double tmp_degree = camera.getTheta();
	tmp_degree += temp_gapX;
	camera.setTheta(tmp_degree);
	//set y change
	double tmp_gapY = ((double)y - (double)oldPosY)*0.1;
	double tmp_Y = camera.getY();
	tmp_Y += tmp_gapY;
	camera.setY(tmp_Y);
	//update pos
	oldPosX = x;
	oldPosY = y;
}

void myIdle()
{
	glutPostRedisplay();
}

int main(int argc, char* argv[])
{
	
	drone.load(filePath);
	drone.setPos(baseFloor.centerX()*0.3, 10, baseFloor.centerZ()*0.5);
	startSphere.setPos(drone.getPosX(), drone.getPosY(), drone.getPosZ());
	startSphere.setRadius(1.0);
	stopSphere.setPos(targetX, targetY, targetZ);
	stopSphere.setRadius(1.0);
	glutInit(&argc, argv);
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);//屏幕大小自适应
	glutMouseFunc(mouseMove); 
	glutKeyboardFunc(handleKeybord);
	glutMotionFunc(changeViewPoint);
	glutIdleFunc(myIdle);
	glutTimerFunc(20 , TimeFunction, 1);
	glutMainLoop();
	return 0;
}
