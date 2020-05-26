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

#include "MySpring.h"
#include "MyAnchoredSpring.h"
#include "MyParticleBuoyancy.h"

class Mover
{
public:
	Mover(int size, int definition,
		float m_mass, float m_damping,
		cyclone::Vector3 position, cyclone::Vector3 velocity,
		cyclone::Vector3 acceleration,
		Color shadow_color, Color obj_color);
	~Mover() = default;

	void update(float duration, bool updateBetweenMovers, bool updateAnchor, bool updateBuoyancy);
	void windBlow();
	void reset();
	void draw(int shadow);
	void setConnection(Mover *other, float spring, int length);
	void setAnchoredConnection(cyclone::Vector3 *anchored, double springConst, double restLength);
	void setParticleBuoyancy(cyclone::real maxDepth, cyclone::real volume, cyclone::real waterHeight, cyclone::real liquidDensity);

	int m_run;
	int m_size;
	int m_definition;

	float m_mass;
	float m_damping;

	cyclone::Vector3 m_position;
	cyclone::Vector3 m_velocity;
	cyclone::Vector3 m_acceleration;

	cyclone::Vector3 m_position_save;
	cyclone::Vector3 m_velocity_save;
	cyclone::Vector3 m_acceleration_save;

	cyclone::Particle *m_particle;

	cyclone::ParticleForceRegistry *m_forces;
	cyclone::ParticleDrag *m_drag;
	cyclone::ParticleGravity *m_gravity;

	std::vector<cyclone::MySpring *> m_spring;

	cyclone::MyAnchoredSpring *m_anchored;

	cyclone::MyParticleBuoyancy *m_buoyancy;

	Color m_shadow_color;
	Color m_obj_color;
};