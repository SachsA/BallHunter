#include "Mover.h"

//==================== Construction methods ====================//

Mover::Mover(MoverType type,
	float size, int definition,
	float mass, float damping,
	cyclone::Vector3 position, cyclone::Vector3 velocity,
	cyclone::Vector3 acceleration,
	Color shadow_color, Color obj_color)
{
	m_type = type;
	m_size = size;
	m_definition = definition;
	m_mass = mass;
	m_damping = damping;
	m_position = position;
	m_velocity = velocity;
	m_acceleration = acceleration;
	m_shadow_color = shadow_color;
	m_obj_color = obj_color;

	m_position_save = m_position;
	m_velocity_save = m_velocity;
	m_acceleration_save = m_acceleration;

	m_particle = new cyclone::Particle();
	m_particle->setMass(m_mass);
	m_particle->setDamping(m_damping);

	m_forces = new cyclone::ParticleForceRegistry();
	m_gravity = new cyclone::ParticleGravity(cyclone::Vector3::GRAVITY);
	m_drag = new cyclone::ParticleDrag(0.1, 0.01);

	m_forces->add(m_particle, m_gravity);
	m_forces->add(m_particle, m_drag);

	reset();
}


//==================== Setup methods ====================//


void Mover::setConnection(Mover *other, float spring, int length)
{
	m_spring.emplace_back(new cyclone::MySpring(other->m_particle, spring, length));
}

void Mover::setAnchoredConnection(cyclone::Vector3 *anchored, double springConst, double restLength)
{
	m_anchored = new cyclone::MyAnchoredSpring(anchored, springConst, restLength);
}

void Mover::setParticleBuoyancy(cyclone::real maxDepth, cyclone::real volume, cyclone::real waterHeight, cyclone::real liquidDensity)
{
	m_buoyancy = new cyclone::MyParticleBuoyancy(m_size, m_mass, maxDepth, volume, waterHeight, liquidDensity);
}


//==================== Core methods ====================//


void Mover::update(float duration, bool updateBetweenMovers, bool updateAnchor, bool updateBuoyancy)
{
	m_forces->updateForces(duration);

	//If you want to attach multiple movers
	if (updateBetweenMovers) {
		for (unsigned int i = 0; i < m_spring.size(); i++) {
			m_spring[i]->updateForce(m_particle, duration);
		}
	}
	
	//If you want to attach movers to anchor
	if (updateAnchor) {
		m_anchored->updateForce(m_particle, duration);
	}

	//If you want to have particle buoyancy
	if (updateBuoyancy) {
		m_buoyancy->updateForce(m_particle, duration);
	}

	m_particle->integrate(duration);
}

void Mover::reset()
{
	m_particle->setPosition(m_position_save);
	m_particle->setVelocity(0, 0, 0);
}

//Add wind blowing
void Mover::windBlow()
{
	cyclone::Vector3 wind(1.0f, 0, 0);
	m_particle->addForce(wind);
}


//==================== Draw methods ====================//


void Mover::draw(int shadow)
{
	m_particle->getPosition(&m_position); //get the current pos of particle

	if (shadow == 1) {
		glColor3f(m_shadow_color.r, m_shadow_color.g, m_shadow_color.b);
	}
	else {
		glColor3f(m_obj_color.r, m_obj_color.g, m_obj_color.b);
	}

	glPushMatrix();

	if (m_type == Sphere)
		drawSphere();
	else if (m_type == Cube)
		drawCube();

	glPopMatrix();
}

void Mover::drawSphere()
{
	glTranslatef(m_position.x, m_position.y, m_position.z);
	glutSolidSphere(m_size, m_definition, m_definition);
}

void Mover::getGLTransform(float matrix[16])
{
	matrix[0] = (float)transformMatrix.data[0];
	matrix[1] = (float)transformMatrix.data[4];
	matrix[2] = (float)transformMatrix.data[8];
	matrix[3] = 0;

	matrix[4] = (float)transformMatrix.data[1];
	matrix[5] = (float)transformMatrix.data[5];
	matrix[6] = (float)transformMatrix.data[9];
	matrix[7] = 0;

	matrix[8] = (float)transformMatrix.data[2];
	matrix[9] = (float)transformMatrix.data[6];
	matrix[10] = (float)transformMatrix.data[10];
	matrix[11] = 0;

	matrix[12] = (float)transformMatrix.data[3];
	matrix[13] = (float)transformMatrix.data[7];
	matrix[14] = (float)transformMatrix.data[11];
	matrix[15] = 1;
}

void Mover::drawCube()
{
	// Get the OpenGL transformation
	GLfloat mat[16];
	getGLTransform(mat); //convert transformMatrix to opengl-friendly 1D array

	glMultMatrixf(mat);
	glutSolidCube(1.0f);
}