#include "Mortar.h"

void Mortar::rotateToTarget(GLdouble _posX, GLdouble _posY, GLdouble _posZ)
{
	//直接计算与x轴的夹角作为rotate的参数(1,0)
	//构造从迫击炮指向飞机的向量在xz平面上的投影
	//因为此处没有做迫击炮炮管的上下转动，所以只考虑在xz平面上的投影
	GLdouble leftX = _posX - posX;
	GLdouble leftZ = _posZ - posX;
//	cout << "----------------" << endl;
//	cout << "mortar info:" << endl;
//	cout << "leftX " << leftX << "leftZ " << leftZ << endl;
	//如果已经在正上方了就不转动了
	if (abs(leftX) < 1.4|| abs(leftZ )< 1.4)
		return;
	GLdouble tmpResult = leftX*1 + leftZ*0;
	GLdouble tmpCos = tmpResult / (sqrt(leftX*leftX + leftZ*leftZ)*1);
	//因为浮点计算的不精确可能会导致bug 所以加了修改值
	if (tmpCos >= 1)
		tmpCos = 1;
	else if (tmpCos <= -1)
		tmpCos = -1;
	GLdouble toRotate = acos(tmpCos) / (2 * 3.14) * 360;
	double xRes = leftZ*1 - 0*leftX;
	if (xRes < 0) toRotate = -toRotate;
	//step3 更新待旋转夹角和朝向
	rotateAngle = toRotate;
	if (rotateAngle > 360)
		rotateAngle -= 360;
	else if (rotateAngle < -360)
		rotateAngle += 360;

//	cout << "Mortar rotate angle: " << rotateAngle << endl;

}

void Mortar::draw(Shader shader)
{
	(this->innerObject)->Draw(shader);
}

void Mortar::loadModel(string path)
{
	this->innerObject = new Model(path);
}