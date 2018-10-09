#include "ObjLoader.h"
#include"Drone.h"
#include"BaseFloor.h"
#include <random>
#include<iostream>
#include"Camera.h"
#include"MarkSphere.h"
using namespace std;
//ģ��·��
string filePath = "F:\\CG\\lab1\\data\\drone\\Drone.obj";
Drone drone;
BaseFloor baseFloor(100, 100);
Camera camera;
MarkSphere startSphere;
MarkSphere stopSphere;
//ʵ���ƶ����۲�ģ���������
static float c = 3.1415926 / 180.0f;
static float r = 60.0f;
static int degree = 90;
static int oldPosY = -1;
static int oldPosX = -1;
//drone���е�Ŀ�ĵ�
GLdouble targetX=30;
GLdouble targetY=30;
GLdouble targetZ=30;

//���ù�Դ
void setLightRes() {
	GLfloat lightPosition[] = { 0.0f, 0.0f, 1.0f, 0.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
	glEnable(GL_LIGHTING); //���ù�Դ
	glEnable(GL_LIGHT0);   //ʹ��ָ���ƹ�
}

//��ʼ��
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
	//��ʾ����������ģ���Ӿ����в���	
	glLoadIdentity(); 	
	setLightRes();		
	//glPushMatrix();
	gluLookAt(camera.getX(), camera.getY(), camera.getZ(),    //�����λ��
		baseFloor.centerX(), 0.0, baseFloor.centerZ(),   //��������
		0.0, 1.0, 0.0);   //�������������ʸ��
	baseFloor.draw();
	startSphere.draw();
	stopSphere.draw();
	drone.draw();
	//glPopMatrix();
	glutSwapBuffers();
}

//time function���ã�
//1. �������ƫ����
//2. ��drone��target��£
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

//�ƶ����360�۲�ģ��
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
	glutReshapeFunc(reshape);//��Ļ��С����Ӧ
	glutMouseFunc(mouseMove); 
	glutMotionFunc(changeViewPoint);
	glutIdleFunc(myIdle);
	glutTimerFunc(20 , TimeFunction, 1);
	glutMainLoop();
	return 0;
}
