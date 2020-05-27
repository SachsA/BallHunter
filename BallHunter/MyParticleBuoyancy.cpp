#include "MyParticleBuoyancy.h"

using namespace cyclone;

MyParticleBuoyancy::MyParticleBuoyancy(float size, float mass, real maxDepth, real volume, real waterHeight, real liquidDensity)
	:
	m_size(size), m_mass(mass), maxDepth(maxDepth), volume(volume),
	waterHeight(waterHeight), liquidDensity(liquidDensity)
{}


void MyParticleBuoyancy::updateForce(Particle *particle, real duration)
{
	cyclone::Vector3 force(0, 0, 0);
	cyclone::Vector3 pos = particle->getPosition();

	double h = pos.y;

	double immergedPos = waterHeight - (h - m_size);

	if (immergedPos < 0)
		return;
	
	cyclone::real d = immergedPos / (2 * m_size);

	if (d < 0)
	{
		return;
	}
	if (d > 1)
	{
		d = 1;
	}

	force.y = d * volume * liquidDensity * -cyclone::Vector3::GRAVITY.y * m_mass;

	particle->addForce(force);
}