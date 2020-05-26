#include "plinks.h"

namespace cyclone {
	class ParticleCollision : public ParticleLink
	{
	public:
		double size; //particle size
	public:
		ParticleCollision(double size);
		virtual unsigned addContact(ParticleContact *contact, unsigned limit) const;
	};
}