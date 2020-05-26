
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

		//If you want to attach movers between each other => set to true
		bool moversBetweenMovers = false;
		//If you want to attach movers to the anchor => set to true
		bool moversToAnchor = true;
		//If you want to have water and buoyancy => set to true
		bool buoyancy = true;

		//Maximum #of collisions possible
		unsigned maxPossibleContact = 3;

		cyclone::ParticleContact  m_contact[3];

		//Container for MyGroundContact
		std::vector<cyclone::ParticleContactGenerator*> m_contactGenerators;

		//Collision resolver(calculate impulse and change velocity and positions)
		cyclone::ParticleContactResolver *m_resolver;

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
		
		int handle(int);
		
		void setProjection(int clearProjection = 1);
		void getMouseNDC(float& x, float& y);
		void setupLight(float x, float y, float z);

		float fieldOfView;
		Viewer *m_viewer;
		
		MoverConnection *m_connection;
};

