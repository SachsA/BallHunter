#include "ParticleCollision.h"
#include <iostream>

using namespace cyclone;

ParticleCollision::ParticleCollision(double size)
{
	this->size = size;
}

unsigned ParticleCollision::addContact(ParticleContact *contact, unsigned limit) const
{
	contact->particle[0] = particle[0];
	contact->particle[1] = particle[1];

	Vector3 n0 = contact->particle[1]->getPosition() - contact->particle[0]->getPosition();

	float distance = n0.magnitude();

	if (distance < size * 2)
	{
		n0.normalise();

		contact->contactNormal = n0 * -1;
		contact->penetration = (size * 2) - distance;
		contact->restitution = 1;

		return 1;
	}

	return 0;
}