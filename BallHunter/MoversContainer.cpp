#include "MoversContainer.h"

MoversContainer::MoversContainer(bool isConnectionToMovers, bool isConnectionToAnchor, bool isBuoyancy)
{
	m_forces = new cyclone::ParticleForceRegistry();

	setIsConnectionToMovers(isConnectionToMovers);
	setIsConnectionToAnchor(isConnectionToAnchor);
	setIsBuoyancy(isBuoyancy);
}

void MoversContainer::setIsConnectionToMovers(bool isConnectionToMovers)
{
	m_isConnectionToMovers = isConnectionToMovers;
}

void MoversContainer::setIsConnectionToAnchor(bool isConnectionToAnchor)
{
	m_isConnectionToAnchor = isConnectionToAnchor;
}

void MoversContainer::setIsBuoyancy(bool isBuoyancy)
{
	m_isBuoyancy = isBuoyancy;
}

void MoversContainer::initForces()
{
	for (unsigned int i = 0; i < m_movers.size(); i++) {
		for (unsigned int j = 0; j < m_movers[i]->m_spring.size(); j++) {
			m_forces->add(m_movers[i]->m_particle, m_movers[i]->m_spring[j]);
		}
	}
}

void MoversContainer::initForcesAnchored()
{
	for (unsigned int i = 0; i < m_movers.size(); i++) {
		m_forces->add(m_movers[i]->m_particle, m_movers[i]->m_anchored);
	}
}

void MoversContainer::update(float duration)
{
	for (unsigned int i = 0; i < m_movers.size(); i++) {
		m_movers[i]->update(duration, m_isConnectionToMovers, m_isConnectionToAnchor, m_isBuoyancy);
	}
}

void MoversContainer::reset()
{
	for (unsigned int i = 0; i < m_movers.size(); i++) {
		m_movers[i]->reset();
	}
}

void MoversContainer::windBlow()
{
	for (unsigned int i = 0; i < m_movers.size(); i++) {
		m_movers[i]->windBlow();
	}
}

void MoversContainer::draw(int shadow)
{
	for (unsigned int i = 0; i < m_movers.size(); i++) {
		if (!shadow)
			glLoadName(i + 1);
		m_movers[i]->draw(shadow);
	}

	//If multiple movers
	if (m_isConnectionToMovers)
		drawConnection();
	
	//If anchor
	if (m_isConnectionToAnchor)
		drawSpringWithAnchor();
}

void MoversContainer::drawConnection()
{
	glColor3f(0, 0, 0);
	glBegin(GL_LINE_STRIP);
	
	for (unsigned int i = 0; i < m_movers.size(); i++) {
		cyclone::Vector3 p = m_movers[i]->m_particle->getPosition();
		glVertex3f(p.x, p.y, p.z);
	}
	
	glEnd();
}

void MoversContainer::drawSpringWithAnchor()
{
	drawStick();

	glColor3f(0, 0, 0);  //Line color
	glLineWidth(3.0f);  //Line Width

	for (unsigned int i = 0; i < m_movers.size(); i++) {
		glPushMatrix();
		glBegin(GL_LINES);

		cyclone::Vector3 particlePos = m_movers[i]->m_particle->getPosition();
		const cyclone::Vector3 *anchoredPos = m_movers[i]->m_anchored->getAnchor();

		glVertex3f(particlePos.x, particlePos.y, particlePos.z); //Starting point
		glVertex3f(anchoredPos->z, anchoredPos->y, anchoredPos->z);  //Ending point
		glEnd();
		glPopMatrix();
	}
}

void MoversContainer::drawStick()
{
	glColor3f(1, 0, 1);  //Line color
	glLineWidth(3.0f);  //Line Width

	glPushMatrix();
	glBegin(GL_LINES);

	glVertex3f(5, 0, 10); //Starting point
	glVertex3f(5, 15, 10);  //Ending point
	glEnd();
	glPopMatrix();
}