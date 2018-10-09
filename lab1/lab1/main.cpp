#include "ObjLoader.h"
#include"Drone.h"
#include"BaseFloor.h"
#include <random>
#include<iostream>
#include"Camera.h"
#include"MarkSphere.h"
using namespace std;
//模型路径
string filePath = "F:\\CG\\lab1\\data\\drone\\Drone.obj";
Drone drone;
BaseFloor baseFloor(100, 100);
Camera camera;
MarkSphere startSphere;
MarkSphere stopSphere;
//实现移动鼠标观察模型所需变量
static float c = 3.1415926 / 180.0f;
static float r = 60.0f;
static int degree = 90;
static int oldPosY = -1;
static int oldPosX = -1;
//drone飞行的目的地
GLdouble targetX=30;
GLdouble targetY=30;
GLdouble targetZ=30;

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
	glEnable(GL_LIGHT1);
	glEnable(GL_DEPTH_TEST);	
	baseFloor.create();
	
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//表示接下来将对模型视景进行操作	
	glLoadIdentity(); 	
	setLightRes();		
	//glPushMatrix();
	gluLookAt(camera.getX(), camera.getY(), camera.getZ(),    //摄像机位置
		baseFloor.centerX(), 0.0, baseFloor.centerZ(),   //焦点坐标
		0.0, 1.0, 0.0);   //摄像机机顶方向矢量
	baseFloor.draw();
	startSphere.draw();
	stopSphere.draw();
	drone.draw();
	//glPopMatrix();
	glutSwapBuffers();
}

//time function作用：
//1. 生成随机偏移量
//2. 将drone向target靠拢
void TimeFunction(int value)
{	
	drone.flyToPos(10.0, 10.0, 20.0, 0);
	glutTimerFunc(20, TimeFunction, 1);
}

void reshape(int width, int height)
{
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(40.0f, (GLdouble)width / (GLdouble)height, 1.0f, 300.0f);
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
	drone.setPos(0, 0, 10);
	startSphere.setPos(drone.getPosX(), drone.getPosY(), drone.getPosZ());
	startSphere.setRadius(1.0);
	stopSphere.setPos(targetX, targetY, targetZ);
	stopSphere.setRadius(1.0);
	glutInit(&argc, argv);
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);//屏幕大小自适应
	glutMouseFunc(mouseMove); 
	glutMotionFunc(changeViewPoint);
	glutIdleFunc(myIdle);
	glutTimerFunc(20 , TimeFunction, 1);
	glutMainLoop();
	return 0;
}
