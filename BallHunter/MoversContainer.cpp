#include "MoversContainer.h"

//==================== Construction methods ====================//


MoversContainer::MoversContainer()
{
	m_forces = new cyclone::ParticleForceRegistry();
}


//==================== Initation methods ====================//


void MoversContainer::initForcesBetweenMovers()
{
	for (unsigned int i = 0; i < m_movers.size(); i++) {
		for (unsigned int j = 0; j < m_movers[i]->m_spring.size(); j++) {
			if (m_movers[i]->m_spring[j] != nullptr)
				m_forces->add(m_movers[i]->m_particle, m_movers[i]->m_spring[j]);
		}
	}
}

void MoversContainer::initForcesAnchored()
{
	for (unsigned int i = 0; i < m_movers.size(); i++) {
		if (m_movers[i]->m_anchored != nullptr)
			m_forces->add(m_movers[i]->m_particle, m_movers[i]->m_anchored);
	}
}


//==================== Core methods ====================//


void MoversContainer::update(float duration)
{
	for (unsigned int i = 0; i < m_movers.size(); i++) {
		if (!checkEdges(i)) {
			m_movers[i]->attachToOtherMovers(duration);
			m_movers[i]->attachToAnchor(duration);
			m_movers[i]->floating(duration);
		}
	}
}

bool MoversContainer::checkEdges(int i)
{
	if (m_movers[i] != m_ball && m_movers[i]->m_isPicked == false) {
		if (m_movers[i]->m_position.x > 800 || m_movers[i]->m_position.x < -800 ||
			m_movers[i]->m_position.z > 800 || m_movers[i]->m_position.z < -800 ||
			m_movers[i]->m_position.y < 0) {
			m_movers.erase(std::remove(m_movers.begin(), m_movers.end(), m_movers[i]), m_movers.end());
			return true;
		}
	}

	return false;
}


//==================== Effect methods ====================//


void MoversContainer::windBlow()
{
	for (unsigned int i = 0; i < m_movers.size(); i++) {
		m_movers[i]->windBlow();
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

	drawConnection();
	drawSpringsToAnchor();
}

void MoversContainer::drawConnection()
{
	glColor3f(0, 0, 0);
	glBegin(GL_LINE_STRIP);

	for (unsigned int i = 0; i < m_movers.size(); i++) {
		if (!m_movers[i]->m_spring.empty()) {
			cyclone::Vector3 p = m_movers[i]->m_particle->getPosition();
			glVertex3f(p.x, p.y, p.z);
		}
	}

	glEnd();
}

void MoversContainer::drawSpringsToAnchor()
{
	glColor3f(0, 0, 0);  //Line color
	glLineWidth(3.0f);  //Line Width

	for (unsigned int i = 0; i < m_movers.size(); i++) {
		if (m_movers[i]->m_anchored != nullptr) {
			glPushMatrix();
			glBegin(GL_LINES);

			cyclone::Vector3 particlePos = m_movers[i]->m_particle->getPosition();
			const cyclone::Vector3* anchoredPos = m_movers[i]->m_anchored->anchor;

			glVertex3f(particlePos.x, particlePos.y, particlePos.z); //Starting point
			glVertex3f(anchoredPos->x, anchoredPos->y, anchoredPos->z);  //Ending point
			glEnd();
			glPopMatrix();
		}
	}
}