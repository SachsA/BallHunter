
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

#include "MoverConnection.h"
#include "MyGroundContact.h"

class MyGlWindow : public Fl_Gl_Window {
	public:
		MyGlWindow(int x, int y, int w, int h);

		std::vector<Vec3f *> history;

		int run;
		int selected = -1;
		int windBlowing;
		Fl_Slider* time;

		float size;

		cyclone::Vector3 previousPoint;

		std::vector<cyclone::ParticleRod *> rods;
		std::vector<cyclone::ParticleCable *> cables;
		std::vector<cyclone::ParticleCableConstraint *> supports;

		//If you want to attach movers between each other => set to true
		bool moversBetweenMovers = false;
		//If you want to attach movers to the anchor => set to true
		bool moversToAnchor = false;
		//If you want to have water and buoyancy => set to true
		bool buoyancy = false;

		cyclone::ParticleWorld *m_world;

		void initObjects();
		
		void update();
		void drawStuff();
		void doPick();
		void reset();

		void attachMultipleMovers();
		void attachMoversToAnchor(cyclone::Vector3 *anchor);
		void setMoversBuoyancy();
	private:
		void draw();
		void drawWaterTank();
		void drawBridge(int shadow);
		
		int handle(int);
		
		void setProjection(int clearProjection = 1);
		void getMouseNDC(float& x, float& y);
		void setupLight(float x, float y, float z);

		float fieldOfView;
		Viewer *m_viewer;
		
		MoverConnection *m_connection;
};

