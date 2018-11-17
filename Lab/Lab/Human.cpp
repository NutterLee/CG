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
	//pickChoice 是一个0到10的值，人走路的时候一般是匀速的，所以加速度应该不怎么变
	//移动策略
	//当产生的int是1的时候改变加速度，生成两个个大小在最大加速度内方向可正可负的x和z方向的加速度
	//根据这个加速度改变速度
	//当生成的int是2的时候将速度变为0，因为人走着走着突然停下来是很正常的事情
	//整个运行过程中都会检查(x,z)是否在(x1,z1),(x2,z2)所框起来的范围内，如果触碰到边界了就使得加速度方向与当前速度方向相反
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
			//如果超过了边界同时速度和加速度同向，就翻转加速度
			if (speedX*accX > 0)
				accX= -accX;
		}
		if (this->posZ + speedZ*scale > max(posZ1, posZ2) || this->posZ + speedZ*scale < min(posZ1, posZ2))
		{
			//如果超过了边界同时速度和加速度同向，就翻转加速度
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
	//走走停停的走向point
	int pickChoice = choice(generatorHuman);
	//0.2的概率向前走动
	if (pickChoice == 2) {
		//求指向目标方向的速度向量
		GLdouble leftX = _posX - posX;
		GLdouble leftZ = _posZ - posZ;
		GLdouble deltaX = speedDe(generatorHuman) / 10.0*maxAcc*(leftX>0?1:-1);
		GLdouble deltaZ =( leftX!=0?deltaX / leftX*leftZ:0);
		posX += deltaX;
		posZ += deltaZ;

		//计算需要转动的角度
		//当速度为0的时候不需要转动角度
		if (deltaX == 0 && deltaZ == 0)
		{
			return;
		}
		cout << "directionX: " << directionX << "directionZ: " << directionZ << endl;
		cout << "deltaX: " << deltaX << ",  deltaZ: " << deltaZ << endl;
		//step1 求向量的乘积，再求其夹角余弦值，随后求角度并转换为0~360的区间
		GLdouble tmpResult = directionX*deltaX + directionZ*deltaZ;
		GLdouble tmpCos = tmpResult / (sqrt(directionX*directionX + directionZ*directionZ)*sqrt(deltaX*deltaX + deltaZ*deltaZ));
		GLdouble toRotate = acos(tmpCos) / (2 * 3.14) * 360;
		
		//step2 计算叉乘来确定角度的正负
		double xRes = deltaX*directionZ - directionX*deltaZ;
		if (xRes < 0) toRotate = -toRotate;
		//step3 更新待旋转夹角和朝向
		rotateAngle = toRotate;
		directionX = deltaX;
		directionZ = deltaZ;
		cout <<"xRes: "<<xRes<< " human rotate angle: " << rotateAngle << endl;
	}
	//cout << "current human pos: " << posX << ", " << posY << ", " << posZ << endl;
	return;
}
