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
	class MyParticleBuoyancy
	{
	public:


		//==================== Construction/Destruction methods ====================//


		MyParticleBuoyancy::MyParticleBuoyancy(float m_size, float mass, real maxDepth, real volume, real waterHeight, real liquidDensity);
		MyParticleBuoyancy() = default;
		~MyParticleBuoyancy() = default;


		//==================== Core methods ====================//


		void updateForce(Particle* particle, real duration);


		//==================== VARIABLES ====================//


		float m_size;
		float m_mass;

		real maxDepth;
		real volume;
		real waterHeight;
		real liquidDensity;
	};
}