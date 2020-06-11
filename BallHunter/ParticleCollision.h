#include "plinks.h"

namespace cyclone {
	class ParticleCollision : public ParticleLink
	{
	public:


		//==================== Construction/Destruction methods ====================//


		ParticleCollision(double size);


		//==================== Core methods ====================//


		virtual unsigned addContact(ParticleContact* contact, unsigned limit) const;


		//==================== VARIABLES ====================//


		double size; //particle size
	};
}