#include "Human.h"
std::default_random_engine generatorHuman;
std::uniform_int_distribution<int> choice(0, 10);
std::uniform_int_distribution<int> accDe(-10, 10);

void Human::draw(Shader shader)
{
	(this->innerObject)->Draw(shader);
}

void Human::loadModel(string path)
{
	this->innerObject =new Model(path);
}

void Human::randomMove()
{
}

void Human::randomMoveWithLimit(GLdouble posX1, GLdouble posZ1, GLdouble posX2, GLdouble posZ2)
{
	//pickChoice ��һ��0��10��ֵ������·��ʱ��һ�������ٵģ����Լ��ٶ�Ӧ�ò���ô��
	//�ƶ�����
	//��������int��1��ʱ��ı���ٶȣ�������������С�������ٶ��ڷ�������ɸ���x��z����ļ��ٶ�
	//����������ٶȸı��ٶ�
	//�����ɵ�int��2��ʱ���ٶȱ�Ϊ0����Ϊ����������ͻȻͣ�����Ǻ�����������
	//�������й����ж�����(x,z)�Ƿ���(x1,z1),(x2,z2)���������ķ�Χ�ڣ�����������߽��˾�ʹ�ü��ٶȷ����뵱ǰ�ٶȷ����෴
	int pickChoice = choice(generatorHuman);
	if (pickChoice == 2) {
		return;
	}
	GLdouble deltaSpeedX = 0;
	GLdouble deltaSpeedZ = 0;
	GLdouble deltaPosX = 0;
	GLdouble deltaPosZ = 0;

	if (pickChoice == 1) {
		accX = accDe(generatorHuman) / 10.0*maxAcc;
		accZ = accDe(generatorHuman) / 10.0*maxAcc;
	}
	else {
		if (this->posX + speedX*scale > max(posX1, posX2) || this->posX + speedX*scale < min(posX1, posX2))
		{
			//��������˱߽�ͬʱ�ٶȺͼ��ٶ�ͬ�򣬾ͷ�ת���ٶ�
			if (speedX*accX > 0)
				accX= -accX;
		}
		if (this->posZ + speedZ*scale > max(posZ1, posZ2) || this->posZ + speedZ*scale < min(posZ1, posZ2))
		{
			//��������˱߽�ͬʱ�ٶȺͼ��ٶ�ͬ�򣬾ͷ�ת���ٶ�
			if (speedZ*accZ > 0)
				accZ= -accZ;
		}		
	}

	speedX += scale*accX;
	speedZ += scale*accZ;
	posX += scale*speedX;
	posZ += scale*speedZ;
}
