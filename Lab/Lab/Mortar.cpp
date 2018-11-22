#include "Mortar.h"

void Mortar::rotateToTarget(GLdouble _posX, GLdouble _posY, GLdouble _posZ)
{
	//ֱ�Ӽ�����x��ļн���Ϊrotate�Ĳ���(1,0)
	//������Ȼ���ָ��ɻ���������xzƽ���ϵ�ͶӰ
	//��Ϊ�˴�û�����Ȼ����ڹܵ�����ת��������ֻ������xzƽ���ϵ�ͶӰ
	GLdouble leftX = _posX - posX;
	GLdouble leftZ = _posZ - posX;
//	cout << "----------------" << endl;
//	cout << "mortar info:" << endl;
//	cout << "leftX " << leftX << "leftZ " << leftZ << endl;
	//����Ѿ������Ϸ��˾Ͳ�ת����
	if (abs(leftX) < 1.4|| abs(leftZ )< 1.4)
		return;
	GLdouble tmpResult = leftX*1 + leftZ*0;
	GLdouble tmpCos = tmpResult / (sqrt(leftX*leftX + leftZ*leftZ)*1);
	//��Ϊ�������Ĳ���ȷ���ܻᵼ��bug ���Լ����޸�ֵ
	if (tmpCos >= 1)
		tmpCos = 1;
	else if (tmpCos <= -1)
		tmpCos = -1;
	GLdouble toRotate = acos(tmpCos) / (2 * 3.14) * 360;
	double xRes = leftZ*1 - 0*leftX;
	if (xRes < 0) toRotate = -toRotate;
	//step3 ���´���ת�нǺͳ���
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