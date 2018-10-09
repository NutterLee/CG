#include"Drone.h"

std::default_random_engine generator;
std::uniform_int_distribution<int> dis(-50, 50);

void Drone::load(string path)
{
	innerObject = ObjLoader(path);
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
		staticMoveZ = -maxPerZ > leftZ ? -maxPerZ : leftZ;
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
}

void Drone::hoverAtPos(GLdouble tarX, GLdouble tarY, GLdouble tarZ)
{
}

void Drone::escapeFromPos(GLdouble posX1, GLdouble posY1, GLdouble posZ1, GLdouble posX2, GLdouble posY2, GLdouble posZ2)
{
}

void Drone::searchInArea(GLdouble posX1, GLdouble posY1, GLdouble posZ1, GLdouble posX2, GLdouble posY2, GLdouble posZ2)
{
}

void Drone::draw()
{	
	posX += toMoveX;
	posY += toMoveY;
	posZ += toMoveZ;
	glTranslatef(this->posX, this->posY, this->posZ);
	innerObject.Draw();
}
