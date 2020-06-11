#include "MoversContainer.h"

//==================== Construction methods ====================//


MoversContainer::MoversContainer(bool isConnectionToMovers, bool isConnectionToAnchor, bool isBuoyancy)
{
	m_forces = new cyclone::ParticleForceRegistry();

	m_isConnectionToMovers = isConnectionToMovers;
	m_isConnectionToAnchor = isConnectionToAnchor;
	m_isBuoyancy = isBuoyancy;
}


//==================== Initation methods ====================//


void MoversContainer::initForcesBetweenMovers()
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


//==================== Core methods ====================//


void MoversContainer::reset()
{
	for (unsigned int i = 0; i < m_movers.size(); i++) {
		m_movers[i]->reset();
	}
}


//==================== Effect methods ====================//


void MoversContainer::attachMoversToEachOther(float duration)
{
	for (unsigned int i = 0; i < m_movers.size(); i++) {
		m_movers[i]->attachToOtherMovers(duration);
	}
}

void MoversContainer::attachMoversToAnchor(float duration)
{
	for (unsigned int i = 0; i < m_movers.size(); i++) {
		m_movers[i]->attachToAnchor(duration);
	}
}

void MoversContainer::windBlow()
{
	for (unsigned int i = 0; i < m_movers.size(); i++) {
		m_movers[i]->windBlow();
	}
}

void MoversContainer::floating(float duration)
{
	for (unsigned int i = 0; i < m_movers.size(); i++) {
		m_movers[i]->floating(duration);
	}
}

//==================== Draw methods ====================//


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
		drawSpringsToAnchor();
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

void MoversContainer::drawSpringsToAnchor()
{
	glColor3f(0, 0, 0);  //Line color
	glLineWidth(3.0f);  //Line Width

	for (unsigned int i = 0; i < m_movers.size(); i++) {
		glPushMatrix();
		glBegin(GL_LINES);

		cyclone::Vector3 particlePos = m_movers[i]->m_particle->getPosition();
		const cyclone::Vector3* anchoredPos = m_movers[i]->m_anchored->anchor;

		glVertex3f(particlePos.x, particlePos.y, particlePos.z); //Starting point
		glVertex3f(anchoredPos->z, anchoredPos->y, anchoredPos->z);  //Ending point
		glEnd();
		glPopMatrix();
	}
}