/**
 * Class:	ParticleSystemVisual
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */
#ifndef Particle_H
#define Particle_H

#include "JDHUtility/Point3f.h"
#include "JDHUtility/Vector3f.h"

using namespace JDHUtility;

namespace WavesVisuals
{
	class Particle
	{
	public:
		typedef struct
		{
			Point3f *pos;
			bool enabled;
		} TrailPoint;

		Particle(void);
		~Particle(void);

		const Vector3f &getDirection(void) const;
		Point3f &getPosition(void);
		float getScale(void) const;
		TrailPoint **getTrail(void) const;
		TrailPoint **getTrail(unsigned int &trailLength, unsigned int &trailPtr) const;
		unsigned int getTrailLength(void) const;
		unsigned int getTrailPtr(void) const;
		float getTwist(void) const;
		bool isEnabled(void) const;
		void resetTrail(void);
		void saveTrailPosition(void);
		void setDirection(const Vector3f &direction);
		void setEnabled(bool enabled);
		void setPosition(const Point3f &position);
		void setScale(float scale);
		
	private:
		static const unsigned int  DEFAULT_TRAIL_LENGTH = 10;
		static const float MAX_TWIST;

		Vector3f direction;
		bool enabled;
		Point3f position;
		float scale;
		TrailPoint **trail;
		unsigned int trailLength;
		unsigned int trailPtr;
		float twist;
	};
}
#endif

