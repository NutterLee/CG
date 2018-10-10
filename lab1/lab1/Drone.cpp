#include"Drone.h"

std::default_random_engine generator;
std::uniform_int_distribution<int> dis(-2, 2);
std::uniform_int_distribution<int> escapeScale(5, 10);
std::uniform_int_distribution<int> accDis(-10, 10);
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
		//当没有到边界时
		if (((tmpPosXMax > posX1&&tmpPosXMax < posX2) || (tmpPosXMax<posX1&&tmpPosXMax>posX2)) &&
			((tmpPosXMin > posX1&&tmpPosXMin < posX2) || (tmpPosXMin<posX1&&tmpPosXMin>posX2)))
		{			
				accX = newAccX;
		}
		else {
			//到达边界了，调整加速度与当前速度的方向相反
			if (newAccX*speedX >= 0) {
				newAccX = -newAccX;
			}
			accX = newAccX;
		}
		GLdouble tmpPosYMax = posY + maxStopLength / 2.0;
		GLdouble tmpPosYMin = posY - maxStopLength / 2.0;
		//当没有到边界时
		if (((tmpPosYMax > posY1&&tmpPosYMax < posY2) || (tmpPosYMax<posY1&&tmpPosYMax>posY2)) &&
			((tmpPosYMin > posY1&&tmpPosYMin < posY2) || (tmpPosYMin<posY1&&tmpPosYMin>posY2)))
		{
			accY = newAccY;
		}
		else {
			//到达边界了，调整加速度与当前速度的方向相反
			if (newAccY*speedY >= 0) {
				newAccY = -newAccY;
			}
			accY = newAccY;
		}
		GLdouble tmpPosZMax = posZ + maxStopLength / 2.0;
		GLdouble tmpPosZMin = posZ - maxStopLength / 2.0;
		//当没有到边界时
		if (((tmpPosZMax > posZ1&&tmpPosZMax < posZ2) || (tmpPosZMax<posZ1&&tmpPosZMax>posZ2)) &&
			((tmpPosZMin > posZ1&&tmpPosZMin < posZ2) || (tmpPosZMin<posZ1&&tmpPosZMin>posZ2)))
		{
			accZ = newAccZ;
		}
		else {
			//到达边界了，调整加速度与当前速度的方向相反
			if (newAccZ*speedZ >= 0) {
				newAccZ = -newAccZ;
			}
			accZ = newAccZ;
		}
		cout << "current acc: " << accX << ", " << accY << ", " << accZ << endl;
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

//在从起始位置到终点位置的直线周围随机取几个点作为临时目标，当到达一个目标后重复上述操作
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
		//计算与上述向量垂直的向量
		//先随机生成一个向量，再求与上述向量的叉乘，就得到了与两个向量都垂直的向量
		int tmpX = randomData(generator);
		int tmpY = randomData(generator);
		int tmpZ = randomData(generator);
		GLdouble resX = dirY*tmpZ - dirZ*tmpY;
		GLdouble resY = dirZ*tmpX - dirX*tmpZ;
		GLdouble resZ = dirX*tmpY - dirY*tmpX;
		//单位化向量
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
