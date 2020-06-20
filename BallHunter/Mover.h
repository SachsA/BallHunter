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


	//==================== Transform methods ====================//


	void translate(cyclone::Vector3 pos);
	void rotate(float x, float y, float z);
	void angularMovement(float duration, float x, float y, float z, double angularDamping);


	//==================== Effect methods ====================//


	void attachToOtherMovers(float duration);
	void attachToAnchor(float duration);
	void floating(float duration);
	void windBlow();


	//==================== Draw methods ====================//


	void draw(int shadow);
	void drawMover();
	void getGLTransform(float matrix[16]);
	void drawAxises();


	//==================== Setup methods ====================//


	void setupConnection(Mover* other, float spring, int length);
	void setupAnchoredConnection(cyclone::Vector3* anchored, double springConst, double restLength);
	void setupParticleBuoyancy(cyclone::real maxDepth, cyclone::real volume, cyclone::real waterHeight, cyclone::real liquidDensity);


	//==================== VARIABLES ====================//


	int m_definition;

	bool m_isPicked = false;

	float m_size;
	float m_mass;
	float m_damping;

	MoverType m_type;

	cyclone::Quaternion m_orientation;
	cyclone::Matrix4 m_transformMatrix;

	cyclone::Vector3 m_position;
	cyclone::Vector3 m_velocity;
	cyclone::Vector3 m_rotation;

	cyclone::Particle* m_particle;

	std::vector<cyclone::MySpring*> m_spring;

	cyclone::MyAnchoredSpring* m_anchored = nullptr;

	cyclone::MyParticleBuoyancy* m_buoyancy = nullptr;

	Color m_shadow_color;
	Color m_obj_color;
};