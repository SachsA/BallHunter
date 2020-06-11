#include <iostream>

#include "MyGroundContact.h"

using namespace cyclone;


//==================== Init methods ====================//


void MyGroundContact::init(cyclone::Particle* p, double size)
{
	if (p)
		particles.emplace_back(p);
	this->size = size;
}


//==================== Core methods ====================//


unsigned MyGroundContact::addContact(cyclone::ParticleContact* contact, unsigned limit) const
{
	unsigned count = 0;

	for (int i = 0; i < particles.size(); i++) {
		cyclone::Particle* p = particles[i];
		cyclone::real y = p->getPosition().y;

		//If collision
		if (y - size < 0) {
			contact->particle[0] = p;
			contact->particle[1] = NULL;
			contact->contactNormal = Vector3(0, 1, 0);
			contact->penetration = abs(y - size);
			contact->restitution = 1;
			count++;
			contact++;
		}
		if (count >= limit) return count;

	}
	return count;
}