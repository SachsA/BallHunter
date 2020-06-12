#include <iostream>

#include "MyGlWindow.h"
#include "drawUtils.h"
#include "timing.h"
#include "ParticleCollision.h"


//==================== Camera constants ====================//


static double DEFAULT_VIEW_POINT[3] = { 0, 30, 100 };
static double DEFAULT_VIEW_CENTER[3] = { 0, 0, 0 };
static double DEFAULT_UP_VECTOR[3] = { 0, 1, 0 };


//==================== Mouse constants ====================//


static int last_push;
int m_pressedMouseButton;
int m_lastMouseX;
int m_lastMouseY;


//==================== Construction methods ====================//


MyGlWindow::MyGlWindow(int x, int y, int w, int h) :
	Fl_Gl_Window(x, y, w, h)
{
	mode(FL_RGB | FL_ALPHA | FL_DOUBLE | FL_STENCIL);

	fieldOfView = 45;

	glm::vec3 viewPoint(DEFAULT_VIEW_POINT[0], DEFAULT_VIEW_POINT[1], DEFAULT_VIEW_POINT[2]);
	glm::vec3 viewCenter(DEFAULT_VIEW_CENTER[0], DEFAULT_VIEW_CENTER[1], DEFAULT_VIEW_CENTER[2]);
	glm::vec3 upVector(DEFAULT_UP_VECTOR[0], DEFAULT_UP_VECTOR[1], DEFAULT_UP_VECTOR[2]);

	float aspect = (w / (float)h);
	m_viewer = new Viewer(viewPoint, viewCenter, upVector, 45.0f, aspect);

	TimingData::init();
	run = 0;

	//Initialize movers and values of these displayed objects
	createMovers();

	setupGround();
	setupCollisions();

	setupMoversBetweenMovers();
	setupMoversToAnchor();
	setupBuoyancy();

	//setupCables();
	//setupRods();
	//setupCableConstraints();
}

void MyGlWindow::createMovers()
{
	Mover* m;

	size = 2;
	int definition = 30;

	float mass = 5.0f;
	float damping = 0.8f;

	cyclone::Vector3 position = cyclone::Vector3(0, 0, 0);
	cyclone::Vector3 velocity = cyclone::Vector3(0, 0, 0);

	Color obj_color = Color(1, 0, 0);
	Color shadow_color = Color(0.1, 0.1, 0.1);

	m_world = new cyclone::ParticleWorld(1000);

	groundContact = new cyclone::MyGroundContact();

	m_container = new MoversContainer(moversBetweenMovers, moversToAnchor, buoyancy);

	position = cyclone::Vector3(0, 15, 10);
	m = new Mover(Cube, size, definition, mass, damping, position, velocity, shadow_color, obj_color);
	m_container->m_movers.emplace_back(m);

	m_world->getParticles().emplace_back(m_container->m_movers[0]->m_particle);
	m_world->getForceRegistry().add(m_container->m_movers[0]->m_particle, new cyclone::ParticleGravity(cyclone::Vector3::GRAVITY));
	m_world->getForceRegistry().add(m_container->m_movers[0]->m_particle, new cyclone::ParticleDrag(0.1, 0.01));
}


//==================== Setup methods ====================//


void MyGlWindow::setupLight(float x, float y, float z)
{
	GLfloat lightPosition[] = { 500, 900.0, 500, 1.0 };
	GLfloat lightPosition2[] = { 1, 0, 0, 0 };
	GLfloat lightPosition3[] = { 0, -1, 0, 0 };

	GLfloat violetLight[] = { 0.5f, 0.1f, .5f, 1.0 };
	GLfloat whiteLight[] = { 1, 1, 1, 1.0 };
	GLfloat whiteLight2[] = { .3, .3, .3, 1.0 };
	GLfloat blueLight[] = { .1f,.1f,.3f,1.0 };

	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHT2);
	glEnable(GL_DEPTH);

	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

	lightPosition[0] = x;
	lightPosition[1] = y;
	lightPosition[2] = z;

	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, whiteLight);

	glLightfv(GL_LIGHT1, GL_POSITION, lightPosition2);
	glLightfv(GL_LIGHT1, GL_AMBIENT, whiteLight2);

	glLightfv(GL_LIGHT2, GL_POSITION, lightPosition3);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, whiteLight);


	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void MyGlWindow::setupProjection(int clearProjection)
{
	glMatrixMode(GL_PROJECTION);
	glViewport(0, 0, w(), h());
	if (clearProjection)
		glLoadIdentity();
	// compute the aspect ratio so we don't distort things
	double aspect = ((double)w()) / ((double)h());
	gluPerspective(fieldOfView, aspect, 1, 1000);

	// put the camera where we want it to be
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	// use the transformation in the ArcBall

	gluLookAt(
		m_viewer->getViewPoint().x, m_viewer->getViewPoint().y, m_viewer->getViewPoint().z,
		m_viewer->getViewCenter().x, m_viewer->getViewCenter().y, m_viewer->getViewCenter().z,
		m_viewer->getUpVector().x, m_viewer->getUpVector().y, m_viewer->getUpVector().z
	);

	glDisable(GL_BLEND);
}

void MyGlWindow::setupGround()
{
	for (size_t i = 0; i < m_container->m_movers.size(); i++)
	{
		groundContact->init(m_container->m_movers[i]->m_particle, size);
	}

	m_world->getContactGenerators().emplace_back(groundContact);
}

void MyGlWindow::setupCollisions()
{
	for (size_t first = 0; first < m_container->m_movers.size(); first++)
	{
		for (size_t second = 0; second < m_container->m_movers.size(); second++) {
			if (first == second)
				continue;

			cyclone::ParticleCollision* particleCollision = new cyclone::ParticleCollision(size);
			particleCollision->size = (m_container->m_movers[first]->m_size + m_container->m_movers[second]->m_size) / 2;

			particleCollision->particle[0] = m_container->m_movers[first]->m_particle;
			particleCollision->particle[1] = m_container->m_movers[second]->m_particle;

			m_world->getContactGenerators().emplace_back(particleCollision);
		}
	}
}

void MyGlWindow::setupMoversBetweenMovers()
{
	if (moversBetweenMovers) {
		if (m_container->m_movers.size() <= 1) {
			moversBetweenMovers = false;
			m_container->m_isConnectionToMovers = false;
		}
		else {
			for (unsigned int i = 0; i < m_container->m_movers.size(); i++) {
				if (i + 1 < m_container->m_movers.size()) {
					m_container->m_movers[i]->setupConnection(m_container->m_movers[i + 1], 20, 3);
					m_container->m_movers[i + 1]->setupConnection(m_container->m_movers[i], 20, 3);
				}
			}
			m_container->initForcesBetweenMovers();
		}
	}
}

void MyGlWindow::setupMoversToAnchor()
{
	cyclone::Vector3* anchor = new cyclone::Vector3(0, 20, 0);

	if (moversToAnchor) {
		if (anchor == nullptr) {
			moversToAnchor = false;
			m_container->m_isConnectionToAnchor = false;
		}
		else {
			for (unsigned int i = 0; i < m_container->m_movers.size(); i++) {
				m_container->m_movers[i]->setupAnchoredConnection(anchor, 5, 5);
			}
			m_container->initForcesAnchored();
		}
	}
}

void MyGlWindow::setupBuoyancy()
{
	if (buoyancy) {
		for (unsigned int i = 0; i < m_container->m_movers.size(); i++) {
			m_container->m_movers[i]->setupParticleBuoyancy(0, 1, 10, 2);
		}
	}
}

void MyGlWindow::setupCables()
{
	int j = 0;

	if (m_container->m_movers.size() <= 1)
		return;

	for (size_t i = 0; i < (m_container->m_movers.size() / 2) - 1; i++)
	{
		cyclone::ParticleCable* cable = new cyclone::ParticleCable();
		cables.emplace_back(cable);

		cable->particle[0] = m_container->m_movers[j]->m_particle;
		cable->particle[1] = m_container->m_movers[j + 2]->m_particle;
		cable->maxLength = 3.0f;
		cable->restitution = 0.1f;

		m_world->getContactGenerators().emplace_back(cable);

		j += 2;
	}

	j = 1;

	for (size_t i = 5; i < m_container->m_movers.size() - 2; i++)
	{
		cyclone::ParticleCable* cable = new cyclone::ParticleCable();
		cables.emplace_back(cable);

		cable->particle[0] = m_container->m_movers[j]->m_particle;
		cable->particle[1] = m_container->m_movers[j + 2]->m_particle;
		cable->maxLength = 3.0f;
		cable->restitution = 0.1f;

		m_world->getContactGenerators().emplace_back(cable);

		j += 2;
	}
}

void MyGlWindow::setupRods()
{
	int j = 0;

	for (size_t i = 0; i < m_container->m_movers.size() / 2; i++)
	{
		cyclone::ParticleRod* rod = new cyclone::ParticleRod();
		rods.emplace_back(rod);

		rod->particle[0] = m_container->m_movers[j]->m_particle;
		rod->particle[1] = m_container->m_movers[j + 1]->m_particle;
		rod->length = 2.0f;

		m_world->getContactGenerators().emplace_back(rod);

		j += 2;
	}
}

void MyGlWindow::setupCableConstraints()
{
	cyclone::Vector3 position;

	for (size_t i = 0; i < m_container->m_movers.size(); i++)
	{
		cyclone::ParticleCableConstraint* support = new cyclone::ParticleCableConstraint();
		supports.emplace_back(support);

		support->particle = m_container->m_movers[i]->m_particle;

		position = m_container->m_movers[i]->m_particle->getPosition();
		position.y = 10;
		support->anchor = position;

		support->restitution = 0.5f;

		if (i == 0 || i == 1 || i == 10 || i == 11) {
			support->maxLength = 3;
		}
		else if (i == 2 || i == 3 || i == 8 || i == 9) {
			support->maxLength = 3.5;
		}
		else if (i == 4 || i == 5 || i == 6 || i == 7) {
			support->maxLength = 4;
		}

		m_world->getContactGenerators().emplace_back(support);
	}
}


//==================== Core methods ====================//


void MyGlWindow::reset()
{
	m_container->reset();
}

void MyGlWindow::update()
{
	TimingData::get().update();

	if (!run) return;

	float duration = (float)TimingData::get().lastFrameDuration * 0.003;

	if (duration <= 0.0f) return;

	if (windBlowing == true)
		m_container->windBlow();

	if (buoyancy)
		m_container->floating(duration);

	if (moversBetweenMovers)
		m_container->attachMoversToEachOther(duration);

	if (moversToAnchor)
		m_container->attachMoversToAnchor(duration);

	m_world->runPhysics(duration);
}


//==================== Draw methods ====================//


void MyGlWindow::draw()
{
	glViewport(0, 0, w(), h());

	// clear the window, be sure to clear the Z-Buffer too
	glClearColor(0.1, 0.1, 0.1, 1);

	glClearStencil(0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glEnable(GL_DEPTH);

	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

	// now draw the ground plane
	setupProjection();
	setupFloor();

	glPushMatrix();
	drawFloor(2000, 200);
	glPopMatrix();

	setupLight(m_viewer->getViewPoint().x, m_viewer->getViewPoint().y, m_viewer->getViewPoint().z);

	//Draw x, y and z axises
	//drawAxises();

	//draw shadow
	setupShadows();
	drawMovers(1);
	unsetupShadows();

	glEnable(GL_LIGHTING);

	//draw objects
	glPushMatrix();
	drawMovers(0);
	glPopMatrix();

	glEnable(GL_COLOR_MATERIAL);

	if (moversToAnchor)
		drawAnchor();

	if (buoyancy)
		drawWaterTank();
}

void MyGlWindow::drawAxises()
{
	glLineWidth(3.0f);
	glBegin(GL_LINES);
	glColor3f(1, 0, 0);

	glVertex3f(0, 0.1, 0);
	glVertex3f(0, 100, 0);

	glColor3f(0, 1, 0);

	glVertex3f(0, 0.1, 0);
	glVertex3f(100, 0.1, 0);

	glColor3f(0, 0, 1);

	glVertex3f(0, 0.1, 0);
	glVertex3f(0, 0.1, 100);
	glEnd();
	glLineWidth(1.0f);
}

void MyGlWindow::drawWall()
{
	polygonf(4, 20., 0., -25., 20., 0., 25., 20., 30., 25., 20., 30., -25.);
}

void MyGlWindow::drawMovers(int shadow)
{
	glLineWidth(2.0);

	m_container->draw(shadow);

	glBegin(GL_LINES);

	drawRods(shadow);
	drawCables(shadow);
	drawSupports(shadow);

	glEnd();

	glLineWidth(1.0);
}

void MyGlWindow::drawAnchor()
{
	glColor3f(1, 0, 1);  //Line color
	glLineWidth(3.0f);  //Line Width

	glPushMatrix();
	glBegin(GL_LINES);

	glVertex3f(0, 0, 0); //Starting point
	glVertex3f(0, 20, 0);  //Ending point
	glEnd();
	glPopMatrix();
}

void MyGlWindow::drawRods(int shadow)
{
	if (shadow)
		glColor3f(0.2, 0.2, 0.2);
	else
		glColor3f(0, 0, 1);
	for (unsigned i = 0; i < rods.size(); i++)
	{
		cyclone::Particle** particles = rods[i]->particle;
		const cyclone::Vector3& p0 = particles[0]->getPosition();
		const cyclone::Vector3& p1 = particles[1]->getPosition();
		glVertex3f(p0.x, p0.y, p0.z);
		glVertex3f(p1.x, p1.y, p1.z);
	}
}

void MyGlWindow::drawCables(int shadow)
{
	if (shadow)
		glColor3f(0.2, 0.2, 0.2);
	else
		glColor3f(0, 1, 0);
	for (unsigned i = 0; i < cables.size(); i++)
	{
		cyclone::Particle** particles = cables[i]->particle;
		const cyclone::Vector3& p0 = particles[0]->getPosition();
		const cyclone::Vector3& p1 = particles[1]->getPosition();
		glVertex3f(p0.x, p0.y, p0.z);
		glVertex3f(p1.x, p1.y, p1.z);
	}
}

void MyGlWindow::drawSupports(int shadow)
{
	if (shadow)
		glColor3f(0.2, 0.2, 0.2);
	else
		glColor3f(0.7f, 0.7f, 0.7f);
	for (unsigned i = 0; i < supports.size(); i++)
	{
		const cyclone::Vector3& p0 = supports[i]->particle->getPosition();
		const cyclone::Vector3& p1 = supports[i]->anchor;
		glVertex3f(p0.x, p0.y, p0.z);
		glVertex3f(p1.x, p1.y, p1.z);
	}
}

void MyGlWindow::drawWaterTank()
{
	glDisable(GL_LIGHTING);
	glEnable(GL_BLEND);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glPushMatrix();
	glColor4f(0, 0, 1, 0.2f);
	glTranslatef(0, 5.0, 0);
	drawCube(100, 10, 100);
	glPopMatrix();
}


//==================== Mouse handling methods ====================//


void MyGlWindow::doPick()
{
	make_current();		// since we'll need to do some GL stuff

	int mx = Fl::event_x(); // where is the mouse?
	int my = Fl::event_y();

	// get the viewport - most reliable way to turn mouse coords into GL coords
	int viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	// set up the pick matrix on the stack - remember, FlTk is
	// upside down!
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPickMatrix((double)mx, (double)(viewport[3] - my), 5, 5, viewport);

	// now set up the projection
	setupProjection(false);

	// now draw the objects - but really only see what we hit
	GLuint buf[100];
	glSelectBuffer(100, buf);
	glRenderMode(GL_SELECT);
	glInitNames();
	glPushName(0);

	m_container->draw(0);

	// go back to drawing mode, and see how picking did
	int hits = glRenderMode(GL_RENDER);
	if (hits) {
		// warning; this just grabs the first object hit - if there
		// are multiple objects, you really want to pick the closest
		// one - see the OpenGL manual 
		// remember: we load names that are one more than the index
		selected = buf[3] - 1;
	}
	else {// nothing hit, nothing selected
		selected = -1;
	}
}

int MyGlWindow::handle(int e)
{
	switch (e) {
	case FL_SHOW:
		show();
		return 1;
	case FL_PUSH:
	{
		m_pressedMouseButton = Fl::event_button();
		m_lastMouseX = Fl::event_x();
		m_lastMouseY = Fl::event_y();

		if (m_pressedMouseButton == 1) {
			doPick();
			damage(1);
			return 1;
		};
		break;
	}
	case FL_RELEASE:
		if (selected >= 0) {
			selected = -1;
			previousPoint = cyclone::Vector3(0, 0, 0);
		}
		m_pressedMouseButton = -1;
		damage(1);
		return 1;
	case FL_DRAG:
	{
		if (selected >= 0 && m_pressedMouseButton == 1) {
			double r1x, r1y, r1z, r2x, r2y, r2z;
			getMouseLine(r1x, r1y, r1z, r2x, r2y, r2z);

			double rx, ry, rz;
			mousePoleGo(r1x, r1y, r1z, r2x, r2y, r2z,
				static_cast<double>(m_container->m_movers[selected]->m_particle->getPosition().x),
				static_cast<double>(m_container->m_movers[selected]->m_particle->getPosition().y),
				static_cast<double>(m_container->m_movers[selected]->m_particle->getPosition().z),
				rx, ry, rz,
				(Fl::event_state() & FL_CTRL) != 0);

			cyclone::Vector3 v(rx, ry, rz);
			if (previousPoint.magnitude() > 0) {
				m_container->m_movers[selected]->m_particle->setVelocity((v - previousPoint) * 40);
			}

			previousPoint = v;

			damage(1);
		}
		else {
			double fractionChangeX = static_cast<double>(Fl::event_x() - m_lastMouseX) / static_cast<double>(this->w());
			double fractionChangeY = static_cast<double>(m_lastMouseY - Fl::event_y()) / static_cast<double>(this->h());

			if (m_pressedMouseButton == 1) {
				m_viewer->rotate(fractionChangeX, fractionChangeY);
			}
			else if (m_pressedMouseButton == 2) {
				m_viewer->zoom(fractionChangeY);
			}
			else if (m_pressedMouseButton == 3) {
				m_viewer->translate(-fractionChangeX, -fractionChangeY, (Fl::event_key(FL_Shift_L) == 0) || (Fl::event_key(FL_Shift_R) == 0));
			}
			else {
				std::cout << "Warning: dragging with unknown mouse button! Nothing will be done" << std::endl;
			}

			m_lastMouseX = Fl::event_x();
			m_lastMouseY = Fl::event_y();
			redraw();

			damage(1);
		}
	}

	return 1;

	case FL_KEYBOARD:
		return 0;

	default:
		return 0;
	}
}

void MyGlWindow::getMouseNDC(float& x, float& y)
{
	// notice, we put everything into doubles so we can do the math
	float mx = (float)Fl::event_x();	// remeber where the mouse went down
	float my = (float)Fl::event_y();

	// we will assume that the viewport is the same as the window size
	float wd = (float)w();
	float hd = (float)h();

	// remember that FlTk has Y going the wrong way!
	my = hd - my;

	x = (mx / wd) * 2.0f - 1.f;
	y = (my / hd) * 2.0f - 1.f;
}