#include "MyAnchoredSpring.h"

using namespace cyclone;


//==================== Construction/Destruction methods ====================//


MyAnchoredSpring::MyAnchoredSpring(cyclone::Vector3* anchor, double springConstant,
	double restLenght)
{
	this->anchor = anchor;
	this->springConstant = springConstant;
	this->restLength = restLenght;
}


//==================== Core methods ====================//


void MyAnchoredSpring::updateForce(Particle* particle, real duration)
{
	Vector3 vectorNormalized = (particle->getPosition() - *anchor).unit();
	float vectorLength = (particle->getPosition() - *anchor).magnitude();

	Vector3 force = vectorNormalized * (-springConstant * (vectorLength - restLength));

	particle->addForce(force);
}