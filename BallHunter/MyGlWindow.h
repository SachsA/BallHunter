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
	void setupMoversBetweenMovers(int begin, int end, float spring, int length);

	void setupCables();
	void setupRods();
	void setupCableConstraints();


	//==================== Core methods ====================//


	void update();
	void restart();


	//==================== Ball Hunter Game methods ====================//


	void createHuntingBall();
	void reload();
	void checkBallDetachFromAnchor();
	void checkMoverIsHit();


	//==================== Draw methods ====================//


	void draw();
	void drawAxises();
	void drawWall();

	void drawMovers(int shadow);

	void drawAnchor();
	void drawWaterTank(int x, int y, int z);

	void drawRods(int shadow);
	void drawCables(int shadow);
	void drawSupports(int shadow);


	//==================== Mouse handling methods ====================//


	void getMouseNDC(float& x, float& y);
	void doPick();

	int handle(int);


	//==================== Text methods ====================//


	void drawStrokeText(char const* string, int x, int y, int z);
	void putText(char const* string, int x, int y, float r, float g, float b);


	//==================== VARIABLES ====================//


	int run;
	int selected;

	float totalTimePrecise;
	int totalTimeSec;

	int score;
	int bestScore;

	int waterHeight = 20;

	float fieldOfView;

	bool windBlowing;
	bool isWaterDrawn = true;
	bool isAnchorDrawn = true;

	Viewer* m_viewer;

	Fl_Slider* time;

	std::vector<Vec3f*> history;

	cyclone::Vector3 previousPoint;
	cyclone::Vector3 *anchor = new cyclone::Vector3(0, 100, 0);

	cyclone::ParticleWorld* m_world;

	cyclone::MyGroundContact* groundContact;

	MoversContainer* m_container;

	std::vector<cyclone::ParticleRod*> rods;
	std::vector<cyclone::ParticleCable*> cables;
	std::vector<cyclone::ParticleCableConstraint*> supports;
};