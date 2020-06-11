
#include <FL/Fl_Gl_Window.h>
#include <Fl/Fl.h>
#include <Fl/Fl_Value_Slider.H>
#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <Fl/Fl_Double_Window.h>

#include "stdio.h"
#include "math.h"
#include "3DUtils.h"

#include "Vec3f.h"
#include "Viewer.h"

#include <vector>
#include <pworld.h>

#include "core.h"

#include "MoversContainer.h"
#include "MyGroundContact.h"

class MyGlWindow : public Fl_Gl_Window {
public:


	//==================== Construction/Destruction methods ====================//


	MyGlWindow(int x, int y, int w, int h);
	void createMovers();


	//==================== Setup methods ====================//


	void setupProjection(int clearProjection = 1);
	void setupLight(float x, float y, float z);

	void setupGround();
	void setupCollisions();
	void setupMoversBetweenMovers();
	void setupMoversToAnchor();
	void setupBuoyancy();

	void setupCables();
	void setupRods();
	void setupCableConstraints();


	//==================== Core methods ====================//


	void update();
	void reset();


	//==================== Draw methods ====================//


	void draw();
	void drawAxises();
	void drawWall();

	void drawMovers(int shadow);

	void drawRods(int shadow);
	void drawCables(int shadow);
	void drawSupports(int shadow);

	void drawAnchor();
	void drawWaterTank();


	//==================== Mouse handling methods ====================//


	void getMouseNDC(float& x, float& y);
	void doPick();

	int handle(int);


	//==================== VARIABLES ====================//


	int run;
	int selected = -1;

	float fieldOfView;
	float size;

	bool windBlowing;

	//If you want to attach movers between each other
	bool moversBetweenMovers = false;
	//If you want to attach movers to the anchor
	bool moversToAnchor = false;
	//If you want to have water and buoyancy
	bool buoyancy = false;

	Viewer* m_viewer;

	Fl_Slider* time;

	std::vector<Vec3f*> history;

	cyclone::Vector3 previousPoint;

	cyclone::ParticleWorld* m_world;

	cyclone::MyGroundContact* groundContact;

	MoversContainer* m_container;

	std::vector<cyclone::ParticleRod*> rods;
	std::vector<cyclone::ParticleCable*> cables;
	std::vector<cyclone::ParticleCableConstraint*> supports;
};