#include <vector>
#include "particle.h"
#include "plinks.h"

namespace cyclone {
	class MyGroundContact : public cyclone::ParticleContactGenerator
	{
	public:


		//==================== Construction/Destruction methods ====================//


		MyGroundContact() = default;
		~MyGroundContact() = default;


		//==================== Init methods ====================//


		void init(cyclone::Particle* p, double size); //add particle p in the container


		//==================== Core methods ====================//


		//Detect collision. If there are collisions, write info in the contact (limit=max # of contacts)
		//If there are collision, return the number of collisions
		virtual unsigned addContact(cyclone::ParticleContact* contact, unsigned limit) const;


		//==================== VARIABLES ====================//


		std::vector<cyclone::Particle*> particles;  //Container for all particles
		double size; //particle size(all particles have a same size)
	};
}