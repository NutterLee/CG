#pragma once
#include"GL/glut.h"
class BaseFloor {
	int displayListId;
	int width;
	int depth;
public:
	BaseFloor(int width, int depth) {
		this->width = width;
		this->depth = depth;
	}
	double centerX() {
		return width / 2.0;
	}
	double centerZ() {
		return depth / 2.0;
	}
	void create() {
		displayListId = glGenLists(1);
		glNewList(displayListId, GL_COMPILE);
		GLfloat lightPos[] = { -30,30,-30,1 };
		glLightfv(GL_LIGHT1, GL_POSITION, lightPos);
		//�����ı���
		glBegin(GL_QUADS);
		//����������
		glNormal3d(0, 1, 0);
		for (int x = 0; x < width - 1; x++) {
			for (int z = 0; z < depth - 1; z++) {
				//����ÿ�����ӵĲ�������
				GLfloat color1[3] = { 1,1,1 };
				GLfloat color2[3] = { 0,0,0 };
				glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE,
					(x + z) % 2 == 0 ? color1 : color2);
				//�ı��ε�4��������
				glVertex3d(x, 0, z);
				glVertex3d(x + 1, 0, z);
				glVertex3d(x + 1, 0, z + 1);
				glVertex3d(x, 0, z + 1);
			}
		}
		glEnd();
		glEndList();
	}
	void draw() {
		glCallList(displayListId);
	}
};