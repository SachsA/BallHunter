#include "MySpring.h"

using namespace cyclone;


//==================== Construction/Destruction methods ====================//


MySpring::MySpring(cyclone::Particle* p, double springConstant, double restLenght)
{
	other = p;
	this->springConstant = springConstant;
	this->restLength = restLenght;
}


//==================== Core methods ====================//


void MySpring::updateForce(Particle* p, real duration)
{
	Vector3 vectorNormalized = (other->getPosition() - p->getPosition()).unit();
	float vectorLength = (other->getPosition() - p->getPosition()).magnitude();

	Vector3 force = vectorNormalized * (-springConstant * (vectorLength - restLength));

	other->addForce(force);
}