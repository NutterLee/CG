#include "ObjLoader.h"
#include <random>
#include<iostream>
using namespace std;
//ģ��·��
string filePath = "F:\\CG\\lab1\\data\\drone\\Drone.obj";


//������������
GLdouble posX = 0;
GLdouble posY = 0;
GLdouble posZ = 0;
//�����ƫ��
GLdouble deltaX = 0;
GLdouble deltaY = 0;
GLdouble deltaZ = 0;
//ÿһ��Ҫ�˶���ƫ��
GLdouble toMoveX = 0;
GLdouble toMoveY = 0;
GLdouble toMoveZ = 0;
//Ŀ�����ڵ�λ��
GLdouble targetX = 100;
GLdouble targetY = 100;
GLdouble targetZ = 100;
//ÿ����Ⱦ���Ĳ���
GLdouble maxPerX = 0.1;
GLdouble maxPerY = 0.1;
GLdouble maxPerZ = 0.1;
std::default_random_engine generator;
std::uniform_int_distribution<int> dis(-2, 2);
ObjLoader objModel = ObjLoader(filePath);
//ʵ���ƶ����۲�ģ���������
static float c = 3.1415926 / 180.0f;
static float r = 60.0f;
static int degree = 90;
static int oldPosY = -1;
static int oldPosX = -1;

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
	glEnable(GL_DEPTH_TEST);
	gluLookAt(r*cos(c*degree), 0, r*sin(c*degree), 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
}

void display()
{
	glColor3f(1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity(); 
	//����λ�ü�¼��Ϣ
	posX += toMoveX;
	posY += toMoveY;
	posZ += toMoveZ;
	glTranslatef(toMoveX, toMoveY, toMoveZ);
	cout << "current pos:" << posX << ", " << posY << ", " << posZ << endl;
	setLightRes();
	glPushMatrix();
	objModel.Draw();//����objģ��
	glPopMatrix();
	glutSwapBuffers();
}

//time function���ã�
//1. �������ƫ����
//2. ��drone��target��£
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

//�ƶ����360�۲�ģ��
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
