#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include "stdio.h"
#include "math.h"
#include "3DUtils.h"
#include "DrawUtils.h"
#include "Particle.h"

#include "Vec3f.h"
#include "Viewer.h"
#include <vector>

#include "core.h"

#include "pfgen.h"

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


	//==================== Effect methods ====================//


	void attachMoversToEachOther(float duration);
	void attachMoversToAnchor(float duration);
	void floating(float duration);
	void windBlow();


	//==================== Draw methods ====================//


	void draw(int shadow);
	void drawConnection();
	void drawSpringsToAnchor();


	//==================== VARIABLES ====================//


	cyclone::ParticleForceRegistry* m_forces;
	std::vector<Mover*> m_movers;
};