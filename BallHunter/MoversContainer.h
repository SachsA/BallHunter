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
	MoversContainer(bool isConnectionToMovers, bool isConnectionToAnchor, bool isBuoyancy);
	MoversContainer() = default;
	~MoversContainer() = default;
	
	void setIsConnectionToMovers(bool isConnectionToMovers);
	void setIsConnectionToAnchor(bool isConnectionToAnchor);
	void setIsBuoyancy(bool isBuoyancy);

	void initForces();
	void initForcesAnchored();
	
	void reset();
	
	void windBlow();
	
	void update(float duration);
	
	void draw(int shadow);
	void drawConnection();
	void drawSpringWithAnchor();
	void drawStick();

	bool m_isConnectionToMovers;
	bool m_isConnectionToAnchor;
	bool m_isBuoyancy;

	cyclone::ParticleForceRegistry * m_forces;
	std::vector<Mover *> m_movers;
};