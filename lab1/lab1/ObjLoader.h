#pragma once
#include "GL\glew.h"
#include "GL\freeglut.h"
#include"GL\glut.h"
#include <vector>
#include <string>
using namespace std;
class ObjLoader {
public:
	ObjLoader(){}
	ObjLoader(string filename);//���캯��
	void Draw();//���ƺ���
private:
	vector<vector<GLfloat>>vSets;//��Ŷ���(x,y,z)����
	vector<vector<GLint>>fSets;//������������������
};