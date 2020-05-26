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
	class MyAnchoredSpring : public ParticleForceGenerator
	{
	protected:
		Vector3 *anchor; /** Anchor position. */
		double springConstant; /** spring constant. */
		double restLength; /** rest length. */

	public:
		MyAnchoredSpring() = default;
		MyAnchoredSpring(Vector3 *anchor, double springConstant, double restLength);
		const Vector3* getAnchor() const { return anchor; }
		virtual void updateForce(Particle *particle, real duration);
	};
}