#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include "stdio.h"
#include "math.h"
#include "3DUtils.h"
#include "DrawUtils.h"
#include "Particle.h"

#include "Vec3f.h"
#include "Viewer.h"
#include <vector>

#include "core.h"

#include "pfgen.h"

namespace cyclone {
	class MySpring : public ParticleForceGenerator  //Force generating class
	{
		double springConstant; //Spring constant
		double restLength;  //lest length
	public:
		cyclone::Particle *other;  //Target particle

		MySpring(cyclone::Particle * p, double springConstant, double restLenght);
		virtual void updateForce(cyclone::Particle * p, double duration);
	};
}