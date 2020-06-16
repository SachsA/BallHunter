#include "Mover.h"

class MoversContainer
{
public:


	//==================== Construction/Destruction methods ====================//


	MoversContainer();
	~MoversContainer() = default;


	//==================== Setup methods ====================//


	void initForcesBetweenMovers();
	void initForcesAnchored();


	//==================== Core methods ====================//


	void reset();
	void update(float duration);
	bool checkEdges(int i);


	//==================== Effect methods ====================//


	void windBlow();


	//==================== Draw methods ====================//


	void draw(int shadow);
	void drawConnection();
	void drawSpringsToAnchor();


	//==================== VARIABLES ====================//


	cyclone::ParticleForceRegistry* m_forces;
	std::vector<Mover*> m_movers;
};