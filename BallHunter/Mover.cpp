#include "Mover.h"


//==================== Constants ====================//


const float degreesToRadians = R_PI / 180;


//==================== Construction methods ====================//


Mover::Mover(MoverType type,
	float size, int definition,
	float mass, float damping,
	cyclone::Vector3 position, cyclone::Vector3 velocity,
	Color shadow_color, Color obj_color)
{
	m_type = type;
	m_size = size;
	m_definition = definition;
	m_mass = mass;
	m_damping = damping;
	m_position = position;
	m_velocity = velocity;
	m_shadow_color = shadow_color;
	m_obj_color = obj_color;

	m_particle = new cyclone::Particle();
	m_particle->setMass(m_mass);
	m_particle->setDamping(m_damping);

	translate(m_position);
	m_particle->setVelocity(m_velocity);
}


//==================== Setup methods ====================//


void Mover::setupConnection(Mover* other, float spring, int length)
{
	m_spring.emplace_back(new cyclone::MySpring(other->m_particle, spring, length));
}

void Mover::setupAnchoredConnection(cyclone::Vector3* anchored, double springConst, double restLength)
{
	m_anchored = new cyclone::MyAnchoredSpring(anchored, springConst, restLength);
}

void Mover::setupParticleBuoyancy(cyclone::real maxDepth, cyclone::real volume, cyclone::real waterHeight, cyclone::real liquidDensity)
{
	m_buoyancy = new cyclone::MyParticleBuoyancy(m_size, m_mass, maxDepth, volume, waterHeight, liquidDensity);
}


//==================== Transform methods ====================//


void Mover::translate(cyclone::Vector3 pos)
{
	m_particle->setPosition(pos);
}

void Mover::rotate(float x, float y, float z)
{
	cyclone::Quaternion qX;
	qX.r = cos(degreesToRadians * x * 0.5f);
	cyclone::Vector3 v = cyclone::Vector3(1, 0, 0) * sin(degreesToRadians * x * 0.5f);
	qX.i = v.x;
	qX.j = v.y;
	qX.k = v.z;
	qX.normalise();

	cyclone::Quaternion qY;
	qY.r = cos(degreesToRadians * y * 0.5f);
	v = cyclone::Vector3(0, 1, 0) * sin(degreesToRadians * y * 0.5f);
	qY.i = v.x;
	qY.j = v.y;
	qY.k = v.z;
	qY.normalise();

	cyclone::Quaternion qZ;
	qZ.r = cos(degreesToRadians * z * 0.5f);
	v = cyclone::Vector3(1, 0, 0) * sin(degreesToRadians * z * 0.5f);
	qZ.i = v.x;
	qZ.j = v.y;
	qZ.k = v.z;
	qZ.normalise();

	qY *= qZ;
	qX *= qY;

	m_orientation *= qX;

	m_transformMatrix.setOrientationAndPos(m_orientation, cyclone::Vector3(0, 0, 0));
}

void Mover::angularMovement(float duration, float x, float y, float z, double angularDamping)
{
	cyclone::Vector3 angularAcceleration(x, y, z); //a fixed angular acc. : 𝜶 (will change it later)
	m_rotation.addScaledVector(angularAcceleration, duration); //𝛚= 𝛚 + 𝜶∙∆t (update angular vel. with angular acc.)

	m_rotation *= real_pow(angularDamping, duration); //𝛚=𝛚∙𝒅𝒂𝒎𝒑𝒊𝒏𝒈 (Multiply damping)

	m_orientation.addScaledVector(m_rotation, duration); //𝒒′=𝒒+𝟏/𝟐 𝒒(𝛚)∗𝒒 (Convert 𝛚 to a quaternion and multiply it

	m_orientation.normalise();

	m_transformMatrix.setOrientationAndPos(m_orientation, cyclone::Vector3(0, 0, 0));
}


//==================== Effect methods ====================//


void Mover::attachToOtherMovers(float duration)
{
	for (unsigned int i = 0; i < m_spring.size(); i++) {
		m_spring[i]->updateForce(m_particle, duration);
	}
}

void Mover::attachToAnchor(float duration)
{
	if (m_anchored != nullptr)
		m_anchored->updateForce(m_particle, duration);
}

void Mover::floating(float duration)
{
	if (m_buoyancy != nullptr)
		m_buoyancy->updateForce(m_particle, duration);
}

void Mover::windBlow()
{
	cyclone::Vector3 wind(10.0f, 0, 0);
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

	drawMover();

	//If you want to draw the axises of the mover
	//if (!shadow)
		//drawAxises();

	glPopMatrix();
}

void Mover::drawMover()
{
	//Get the OpenGL transformation
	GLfloat mat[16];
	getGLTransform(mat); //Convert transformMatrix to opengl-friendly 1D array

	glMultMatrixf(mat);

	glTranslatef(m_position.x, m_position.y, m_position.z);

	if (m_type == Sphere)
		glutSolidSphere(m_size, m_definition, m_definition);
	else if (m_type == Cube)
		glutSolidCube(m_size * 2);
}

void Mover::getGLTransform(float matrix[16])
{
	matrix[0] = (float)m_transformMatrix.data[0];
	matrix[1] = (float)m_transformMatrix.data[4];
	matrix[2] = (float)m_transformMatrix.data[8];
	matrix[3] = 0;

	matrix[4] = (float)m_transformMatrix.data[1];
	matrix[5] = (float)m_transformMatrix.data[5];
	matrix[6] = (float)m_transformMatrix.data[9];
	matrix[7] = 0;

	matrix[8] = (float)m_transformMatrix.data[2];
	matrix[9] = (float)m_transformMatrix.data[6];
	matrix[10] = (float)m_transformMatrix.data[10];
	matrix[11] = 0;

	matrix[12] = (float)m_transformMatrix.data[3];
	matrix[13] = (float)m_transformMatrix.data[7];
	matrix[14] = (float)m_transformMatrix.data[11];
	matrix[15] = 1;
}

void Mover::drawAxises()
{
	// Draw axises from cube
	glLineWidth(2.0f);
	glBegin(GL_LINES);
	glColor3f(1, 0, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 10, 0);

	glColor3f(0, 1, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(10, 0, 0);

	glColor3f(0, 0, 1);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, 10);

	glEnd();
	glLineWidth(1.0f);
}