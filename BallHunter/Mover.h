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
#include "MoverType.h"

class Mover
{
public:


	//==================== Construction/Destruction methods ====================//


	Mover(MoverType type,
		float size, int definition,
		float m_mass, float m_damping,
		cyclone::Vector3 position, cyclone::Vector3 velocity,
		Color shadow_color, Color obj_color);
	~Mover() = default;


	//==================== Core methods ====================//


	void reset();


	//==================== Effect methods ====================//


	void rotate(float x, float y, float z);


	//==================== Effect methods ====================//


	void attachToOtherMovers(float duration);
	void attachToAnchor(float duration);
	void floating(float duration);
	void windBlow();


	//==================== Draw methods ====================//


	void draw(int shadow);
	void drawSphere();

	void getGLTransform(float matrix[16]);
	void drawCube();
	void drawAxises();


	//==================== Setup methods ====================//


	void setupConnection(Mover* other, float spring, int length);
	void setupAnchoredConnection(cyclone::Vector3* anchored, double springConst, double restLength);
	void setupParticleBuoyancy(cyclone::real maxDepth, cyclone::real volume, cyclone::real waterHeight, cyclone::real liquidDensity);


	//==================== VARIABLES ====================//


	int m_run;
	int m_definition;

	float m_size;
	float m_mass;
	float m_damping;

	MoverType m_type;

	cyclone::Quaternion orientation;
	cyclone::Matrix4 transformMatrix;

	cyclone::Vector3 m_position;
	cyclone::Vector3 m_velocity;

	cyclone::Vector3 m_position_save;
	cyclone::Vector3 m_velocity_save;

	cyclone::Particle* m_particle;

	std::vector<cyclone::MySpring*> m_spring;

	cyclone::MyAnchoredSpring* m_anchored;

	cyclone::MyParticleBuoyancy* m_buoyancy;

	Color m_shadow_color;
	Color m_obj_color;
};