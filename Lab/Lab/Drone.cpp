#include "Drone.h"
#include<ctime>
static std::default_random_engine generator(time(0));
static std::uniform_int_distribution<int> dis(-2, 2);
static std::uniform_int_distribution<int> escapeScale(5, 10);
static std::uniform_int_distribution<int> accDis(-10, 10);
//���ɵļ��ٶȲ���̫С
static std::uniform_int_distribution<int> accDis2(5, 10);
static std::uniform_int_distribution<int> randomData(-10, 10);

void Drone::loadModel(string path)
{
	innerObject = new Model(path.c_str());
}



void Drone::reset()
{
	//��ǰ����
	posX = 0.0;
	posY = 0.0;
	posZ = 0.0;
	//Ŀ������
	targetX = 0;
	targetY = 0;
	targetZ = 0;
	//ÿһ֡��drone����ά�������ٶ�
	maxSpeedX = 1;
	maxSpeedY = 1;
	maxSpeedZ = 1;
	//ÿһ֡��ά��drone�����ļ��ٶȱ仯ֵ������ֵ��
	maxAccX = 0.2;
	maxAccY = 0.2;
	maxAccZ = 0.2;
	//ÿһ֡�ļ��ٶȵ�ֵ
	accX = 0.1;
	accY = 0.1;
	accZ = 0.1;
	//drone����ά�ϵ��ٶ�
	speedX = 0;
	speedY = 0;
	speedZ = 0;
	//��һ�μ��ٶȱ仯���λ��
	lastChangeAccPosX = 999;
	lastChangeAccPosY = 999;
	lastChangeAccPosZ = 999;
	//������ͣ�ĵ���������ƫ����
	maxHoverLength = 5.0;
	// ����mode,state 0��ʾ�ǹ���mode��1��ʾ����mode
	mode = 0;
	//����mode�·������ٿ�����������
	maxAttackLength = 10.0;
	moveScale = 100.0;
	//�������Ľӽ����
	maxStopLength = 0.2;
	//ÿһ֡��ʱ����
	timeSlice = 20 / 100.0;

	//����ʹ�õĲ���
	escapeFinish = true;
	hasEscaped = false;
}

void Drone::changeAcc()
{
	generator.seed(time(0));
	if (needChangeAcc()) {
		accX = accDis(generator) / 10.0*maxAccX;
		accY = accDis(generator) / 10.0*maxAccY;
		accZ = accDis(generator) / 10.0*maxAccZ;
		updateLastChangeAccPos();
	}
}

void Drone::changeAccWithTargeDirect(GLdouble verX, GLdouble verY, GLdouble verZ)
{
	if (needChangeAcc()) {
		GLdouble newAccX = accDis(generator) / 10.0*maxAccX;
		if (newAccX *verX < 0) {
			newAccX = -newAccX;
		}
		GLdouble newAccY = accDis(generator) / 10.0*maxAccY;
		if (newAccY *verY < 0) {
			newAccY = -newAccY;
		}
		GLdouble newAccZ = accDis(generator) / 10.0*maxAccZ;
		if (newAccZ * verZ < 0) {
			newAccZ = -newAccZ;
		}

		//Ҫ�����ɼн�Ϊ30�����ϵ�ֵ
		//����н�
		GLdouble cosAng = 1;
		cosAng = (newAccX*verX + newAccY*verY +
			newAccZ*verZ) / (sqrt(newAccX*newAccX + newAccY*newAccY + newAccZ*newAccZ)*
				sqrt(verX*verX + verY*verY + verZ*verZ));
		while (cosAng>0.7)
		{
			newAccX = accDis(generator) / 10.0*maxAccX;
			if (newAccX *verX < 0) {
				newAccX = -newAccX;
			}
			newAccY = accDis(generator) / 10.0*maxAccY;
			if (newAccY *verY < 0) {
				newAccY = -newAccY;
			}
			newAccZ = accDis(generator) / 10.0*maxAccZ;
			if (newAccZ * verZ < 0) {
				newAccZ = -newAccZ;
			}
			cosAng = (newAccX*verX + newAccY*verY +
				newAccZ*verZ) / (sqrt(newAccX*newAccX + newAccY*newAccY + newAccZ*newAccZ)*
					sqrt(verX*verX + verY*verY + verZ*verZ));
		}
		accX = newAccX;
		accY = newAccY;
		accZ = newAccZ;
		updateLastChangeAccPos();
		//cout << "now acc: " << accX << "," << accY << "," << accZ << endl;
	}
}

void Drone::changeAccWithLimitSpace(GLdouble posX1, GLdouble posY1, GLdouble posZ1, GLdouble posX2, GLdouble posY2, GLdouble posZ2)
{
	if (needChangeAcc()) {
		GLdouble newAccX = accDis(generator) / 10.0*maxAccX;
		GLdouble newAccY = accDis(generator) / 10.0*maxAccY;
		GLdouble newAccZ = accDis(generator) / 10.0*maxAccZ;
		GLdouble tmpPosXMax = posX + maxStopLength / 2.0;
		GLdouble tmpPosXMin = posX - maxStopLength / 2.0;
		bool hasChangeSpeed = false;
		//�����߽�ʱ
		if (tmpPosXMax<min(posX1, posX2) || tmpPosXMax>max(posX1, posX2) || tmpPosXMin<min(posX1, posX2) || tmpPosXMin>max(posX1, posX2))
		{
			//����߽��ˣ��������ٶ�ָ�����ķ���
			//cout << "hit x !" << endl;
			GLdouble centerX = (posX1 + posX2) / 2.0;
			if (newAccX*(centerX - posX) <= 0) {
				newAccX = -newAccX;
			}
			accX = newAccX;
			hasChangeSpeed = true;
		}
		GLdouble tmpPosYMax = posY + maxStopLength / 2.0;
		GLdouble tmpPosYMin = posY - maxStopLength / 2.0;
		//��û�е��߽�ʱ
		if (tmpPosYMax<min(posY1, posY2) || tmpPosYMax>max(posY1, posY2) || tmpPosYMin<min(posY1, posY2) || tmpPosYMin>max(posY1, posY2))
		{
			//����߽���
			GLdouble centerY = (posY1 + posY2) / 2.0;
			if (newAccY*(centerY - posY) <= 0) {
				newAccY = -newAccY;
			}
			accY = newAccY;
			//cout << "hit y!" << endl;
			hasChangeSpeed = true;
		}
		GLdouble tmpPosZMax = posZ + maxStopLength / 2.0;
		GLdouble tmpPosZMin = posZ - maxStopLength / 2.0;
		//��û�е��߽�ʱ
		if (tmpPosZMax<min(posZ1, posZ2) || tmpPosZMax>max(posZ1, posZ2) || tmpPosZMin<min(posZ1, posZ2) || tmpPosZMin>max(posZ1, posZ2))
		{
			//����߽��ˣ��������ٶ��뵱ǰ�ٶȵķ����෴
			GLdouble centerZ = (posZ1 + posZ2) / 2.0;
			if (newAccZ*(centerZ - posZ) <= 0) {
				newAccZ = -newAccZ;
			}
			accZ = newAccZ;
			hasChangeSpeed = true;
			//cout << "hit z!" << endl;
		}
		//cout << "current acc: " << accX << ", " << accY << ", " << accZ << endl;
		if (hasChangeSpeed)
			updateLastChangeAccPos();
	}
}


//ȫ�ٶ�ֱ�߽ӽ�
void Drone::flyToPos(GLdouble tarX, GLdouble tarY, GLdouble tarZ, int mode)
{
	this->mode = mode;
	this->targetX = tarX;
	this->targetY = tarY;
	this->targetZ = tarZ;
	GLdouble leftX = targetX - posX;
	GLdouble leftY = targetY - posY;
	GLdouble leftZ = targetZ - posZ;
	if (abs(leftX) < maxStopLength&&abs(leftX) < maxStopLength&&abs(leftX) < maxStopLength) {
		speedX = 0;
		speedY = 0;
		speedZ = 0;
	}
	else {
		//��λ����������
		GLdouble dirLength = sqrt(leftX*leftX + leftY*leftY + leftZ*leftZ);
		leftX = leftX / dirLength;
		leftY = leftY / dirLength;
		leftZ = leftZ / dirLength;
		GLdouble maxSpeedScale = sqrt(maxSpeedX*maxSpeedX + maxSpeedY*maxSpeedY + maxSpeedZ*maxSpeedZ);
		speedX = leftX*maxSpeedScale;
		speedY = leftY*maxSpeedScale;
		speedZ = leftZ*maxSpeedScale;
		//cout<<"in "
		changePos();
	}
}

//�ڴ���ʼλ�õ��յ�λ�õ�ֱ����Χ���ȡ��������Ϊ��ʱĿ�꣬������һ��Ŀ����ظ���������
void Drone::flyToPos2(GLdouble tarX, GLdouble tarY, GLdouble tarZ, int mode)
{
	this->mode = mode;
	this->targetX = tarX;
	this->targetY = tarY;
	this->targetZ = tarZ;

	GLdouble leftTmpX = tarX - posX;
	GLdouble leftTmpY = tarY - posY;
	GLdouble leftTmpZ = tarZ - posZ;
	//cout << "left ver: " << leftTmpX << ", " << leftTmpY << ", " << leftTmpZ << endl;
	changeAccWithTargeDirect(leftTmpX, leftTmpY, leftTmpZ);
	changeSpeed();
	changePos();
	//cout << "current pos: " << posX << ", " << posY << ", " << posZ << endl;
}

void Drone::hoverAtPos(GLdouble tarX, GLdouble tarY, GLdouble tarZ)
{
	int scale = 30;
	GLdouble posX1 = tarX + maxStopLength * scale;
	GLdouble posY1 = tarY + maxStopLength * scale;
	GLdouble posZ1 = tarZ + maxStopLength*scale;
	GLdouble posX2 = tarX - maxStopLength*scale;
	GLdouble posY2 = tarY - maxStopLength*scale;
	GLdouble posZ2 = tarZ - maxStopLength*scale;
	//cout << "limit space:" << posX1 << ", " << posY1 << ", " << posZ1 << "  "
	//	<< posX2 << ", " << posY2 << ", " << posZ2 << endl;
	changeAccWithLimitSpace(posX1, posY1, posZ1, posX2, posY2, posZ2);
	changeSpeed();
	changePos();
	//cout << "current drone pos: " << posX << ", " << posY << ", " << posZ << endl;
}

void Drone::escapeFromPos(GLdouble posX1, GLdouble posY1, GLdouble posZ1, GLdouble posX2, GLdouble posY2, GLdouble posZ2)
{
	if (escapeFinish == true && hasEscaped == false) {
		GLdouble dirX = posX1 - posX2;
		GLdouble dirY = posY1 - posY2;
		GLdouble dirZ = posZ1 - posZ2;
		//����������������ֱ������
		//���������һ�����������������������Ĳ�ˣ��͵õ�����������������ֱ������
		int tmpX = randomData(generator);
		int tmpY = randomData(generator);
		int tmpZ = randomData(generator);
		GLdouble resX = dirY*tmpZ - dirZ*tmpY;
		GLdouble resY = dirZ*tmpX - dirX*tmpZ;
		GLdouble resZ = dirX*tmpY - dirY*tmpX;
		//��λ������
		GLdouble resLength = sqrtl(resX*resX + resY*resY + resZ*resZ);
		resX = resX / resLength;
		resY = resY / resLength;
		resZ = resZ / resLength;
		int scale = escapeScale(generator);
		resX = posX2 + resX*scale;
		resY = posY2 + resY*scale;
		resZ = posZ2 + resZ*scale;
		//cout << "fly to pos: " << resX << ", " << resY << ", " << resZ << endl;
		targetX = resX;
		targetY = resY;
		targetZ = resZ;
		flyToPos2(resX, resY, resZ, 0);
	}
	else {
		if (abs(posX - targetX) < maxStopLength * 10 && abs(posY - targetY) < maxStopLength * 10 && abs(posZ - targetZ) < maxStopLength * 10) {
			escapeFinish = true;
		}
		else flyToPos2(targetX, targetY, targetZ, 0);
	}
	hasEscaped = true;
}

void Drone::searchInArea(GLdouble posX1, GLdouble posY1, GLdouble posZ1, GLdouble posX2, GLdouble posY2, GLdouble posZ2)
{
	changeAccWithLimitSpace(posX1, posY1, posZ1, posX2, posY2, posZ2);
	changeSpeed();
	changePos();
}

void Drone::falldown()
{
	//�Ѿ�׹�䵽������
	if (posY <= 0) {
		speedY = 0;
		accY = 0;
		posY = 0;
		return;
	}
	
	static GLdouble dropAcc = -0.003;
	speedX = 0;
	speedZ = 0;
	speedY += dropAcc;
	changePos();
}

void Drone::draw(Shader shader)
{
	innerObject->Draw(shader);
}

bool Drone::hasFound(GLdouble _posX, GLdouble _posY, GLdouble _posZ)
{
	//����λ�� �˵�ͷλ����ģ�ͻ�׼��y+3.0��λ��
	GLdouble tPosX = _posX;
	GLdouble tPosY = _posY + 3.0;
	GLdouble tPosZ = _posZ;
	GLdouble tmpDis = sqrt((tPosX - posX)*(tPosX - posX) + (tPosY - posY)*(tPosY - posY) + (tPosZ - posZ)*(tPosZ - posZ));
	//cout << "drone pos: " << posX << ", " << posY << " ," << posZ << endl;
//	cout << "human pos: " << _posX << ", " << _posY << ", " << _posZ << endl;
//	cout << "dis: " << tmpDis << endl;
	if (tmpDis < maxDetectLengrh)
		return true;
	return false;
}

GLdouble min(GLdouble x, GLdouble y)
{
	if (x<y)
		return x;
	else return y;
}

GLdouble max(GLdouble x, GLdouble y)
{
	if (x > y)
		return x;
	else return y;
}
