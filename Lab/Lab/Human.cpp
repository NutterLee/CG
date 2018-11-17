#include "Human.h"
std::default_random_engine generatorHuman;
std::uniform_int_distribution<int> choice(0, 20);
std::uniform_int_distribution<int> accDe(-10, 10);
std::uniform_int_distribution<int> speedDe(0,10);

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

void Human::randomMoveToPoint(GLdouble _posX, GLdouble _posZ)
{
	//����ͣͣ������point
	int pickChoice = choice(generatorHuman);
	//0.2�ĸ�����ǰ�߶�
	if (pickChoice == 2) {
		//��ָ��Ŀ�귽����ٶ�����
		GLdouble leftX = _posX - posX;
		GLdouble leftZ = _posZ - posZ;
		GLdouble deltaX = speedDe(generatorHuman) / 10.0*maxAcc*(leftX>0?1:-1);
		GLdouble deltaZ =( leftX!=0?deltaX / leftX*leftZ:0);
		posX += deltaX;
		posZ += deltaZ;

		//������Ҫת���ĽǶ�
		//���ٶ�Ϊ0��ʱ����Ҫת���Ƕ�
		if (deltaX == 0 && deltaZ == 0)
		{
			return;
		}
		cout << "directionX: " << directionX << "directionZ: " << directionZ << endl;
		cout << "deltaX: " << deltaX << ",  deltaZ: " << deltaZ << endl;
		//step1 �������ĳ˻���������н�����ֵ�������ǶȲ�ת��Ϊ0~360������
		GLdouble tmpResult = directionX*deltaX + directionZ*deltaZ;
		GLdouble tmpCos = tmpResult / (sqrt(directionX*directionX + directionZ*directionZ)*sqrt(deltaX*deltaX + deltaZ*deltaZ));
		GLdouble toRotate = acos(tmpCos) / (2 * 3.14) * 360;
		
		//step2 ��������ȷ���Ƕȵ�����
		double xRes = deltaX*directionZ - directionX*deltaZ;
		if (xRes < 0) toRotate = -toRotate;
		//step3 ���´���ת�нǺͳ���
		rotateAngle = toRotate;
		directionX = deltaX;
		directionZ = deltaZ;
		cout <<"xRes: "<<xRes<< " human rotate angle: " << rotateAngle << endl;
	}
	//cout << "current human pos: " << posX << ", " << posY << ", " << posZ << endl;
	return;
}
