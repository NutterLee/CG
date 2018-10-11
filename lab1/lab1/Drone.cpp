#include"Drone.h"

std::default_random_engine generator;
std::uniform_int_distribution<int> dis(-2, 2);
std::uniform_int_distribution<int> escapeScale(5, 10);
std::uniform_int_distribution<int> accDis(-10, 10);
//���ɵļ��ٶȲ���̫С
std::uniform_int_distribution<int> accDis2(5, 10);
std::uniform_int_distribution<int> randomData(-10, 10);

void Drone::load(string path)
{
	innerObject = ObjLoader(path);
}

void Drone::changeAcc()
{
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
		cout << "now acc: " << accX << "," << accY << "," << accZ << endl;
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
		if (tmpPosXMax<min(posX1,posX2)||tmpPosXMax>max(posX1,posX2)||tmpPosXMin<min(posX1,posX2)||tmpPosXMin>max(posX1,posX2))
		 {
			//����߽��ˣ��������ٶ�ָ�����ķ���
			cout << "hit x !" << endl;
			GLdouble centerX = (posX1 + posX2) / 2.0;
			if (newAccX*(centerX-posX) <= 0) {
				newAccX = -newAccX;
			}
			accX = newAccX;
			hasChangeSpeed = true;
		}
		GLdouble tmpPosYMax = posY + maxStopLength / 2.0;
		GLdouble tmpPosYMin = posY - maxStopLength / 2.0;
		//��û�е��߽�ʱ
		if (tmpPosYMax<min(posY1, posY2) || tmpPosYMax>max(posY1, posY2) || tmpPosYMin<min(posY1,posY2) || tmpPosYMin>max(posY1, posY2))
		{
			//����߽���
			GLdouble centerY = (posY1 + posY2) / 2.0;
			if (newAccY*(centerY-posY)<= 0) {
				newAccY = -newAccY;
			}
			accY = newAccY;
			cout << "hit y!" << endl;
			hasChangeSpeed = true;
		}
		GLdouble tmpPosZMax = posZ + maxStopLength / 2.0;
		GLdouble tmpPosZMin = posZ - maxStopLength / 2.0;
		//��û�е��߽�ʱ
		if (tmpPosZMax<min(posZ1, posZ2) || tmpPosZMax>max(posZ1, posZ2) || tmpPosZMin<min(posZ1, posZ2) || tmpPosZMin>max(posZ1, posZ2))
		 {
			//����߽��ˣ��������ٶ��뵱ǰ�ٶȵķ����෴
			GLdouble centerZ = (posZ1 + posZ2) / 2.0;
			if (newAccZ*(centerZ-posZ) <= 0) {
				newAccZ = -newAccZ;
			}
			accZ = newAccZ;
			hasChangeSpeed = true;
			cout << "hit z!" << endl;
		}
		cout << "current acc: " << accX << ", " << accY << ", " << accZ << endl;
		if (hasChangeSpeed)
			updateLastChangeAccPos();
	}	
}

void Drone::changeAccWithTargePos(GLdouble _tarX, GLdouble _tarY, GLdouble _tarZ)
{

}

void Drone::flyToPos(GLdouble tarX, GLdouble tarY, GLdouble tarZ, int mode)
{
	this->mode = mode;
	this->targetX = tarX;
	this->targetY = tarY;
	this->targetZ = tarZ;
	GLdouble deltaX = dis(generator) / moveScale;
	GLdouble deltaY = dis(generator) / moveScale;
	GLdouble deltaZ = dis(generator) / moveScale;
	GLdouble leftX = targetX - posX;
	GLdouble leftY = targetY - posY;
	GLdouble leftZ = targetZ - posZ;
	GLdouble staticMoveX = 0;
	GLdouble staticMoveY = 0;
	GLdouble staticMoveZ = 0;	
	if (leftX > 0) {
		staticMoveX = maxSpeedX > leftX ? leftX : maxSpeedX;
	}
	else if (leftX < 0) {
		staticMoveX = -maxSpeedX > leftX ? -maxSpeedX : leftX;
	}
	if (leftY > 0) {
		staticMoveY = maxSpeedY > leftY ? leftY : maxSpeedY;
	}
	else if (leftY < 0) {
		staticMoveY = -maxSpeedY > leftY ? -maxSpeedY : leftY;
	}
	if (leftZ > 0) {
		staticMoveZ = maxSpeedZ > leftZ ? leftZ : maxSpeedZ;
	}
	else if (leftZ < 0) {
		staticMoveZ = -maxSpeedZ > leftZ ? -maxSpeedZ : leftZ;
	}
	if (mode == 1 && (leftX*leftX + leftY*leftY + leftZ*leftZ <= maxAttackLength*maxAttackLength)) {
		toMoveX = staticMoveX;
		toMoveY = staticMoveY;
		toMoveZ = staticMoveZ;
	}
	else {
		toMoveX = staticMoveX + deltaX;
		toMoveY = staticMoveY + deltaY;
		toMoveZ = staticMoveZ + deltaZ;
	}	
	if (leftX == 0) {
		toMoveX = 0;
	}
	if (leftY == 0) {
		toMoveY = 0;
	}
	if (leftZ == 0) {
		toMoveZ = 0;
	}
	posX += toMoveX;
	posY += toMoveY;
	posZ += toMoveZ;
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
	cout << "left ver: " << leftTmpX << ", " << leftTmpY << ", " << leftTmpZ << endl;
	changeAccWithTargeDirect(leftTmpX,leftTmpY, leftTmpZ);
	changeSpeed();
	changePos();
	cout << "current pos: " << posX << ", " << posY << ", " << posZ << endl;
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
	cout << "current pos: " << posX << ", " << posY << ", " << posZ << endl;
}

void Drone::escapeFromPos(GLdouble posX1, GLdouble posY1, GLdouble posZ1, GLdouble posX2, GLdouble posY2, GLdouble posZ2)
{
	if (escapeFinish == true) {
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
		cout << "fly to pos: " << resX << ", " << resY << ", " << resZ << endl;
		flyToPos2(resX, resY, resZ, 0);
	}
	else {		
		if (abs(posX - targetX) < maxStopLength * 10 && abs(posY - targetY) < maxStopLength * 10 && abs(posZ - targetZ) < maxStopLength * 10) {
			escapeFinish = true;
		}
		else flyToPos2(targetX, targetY, targetZ,0);
	}
	
}

void Drone::searchInArea(GLdouble posX1, GLdouble posY1, GLdouble posZ1, GLdouble posX2, GLdouble posY2, GLdouble posZ2)
{
}

void Drone::draw()
{		
	glTranslatef(this->posX, this->posY, this->posZ);
	innerObject.Draw();
}

GLdouble min(GLdouble x, GLdouble y)
{
	if(x<y)
	return x;
	else return y;
}

GLdouble max(GLdouble x, GLdouble y)
{
	if (x > y)
		return x;
	else return y;
}
