/**
 * Class:	ParticleSystemVisual
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */
#include <assert.h>
#include <stdlib.h>
#include "Particle.h"

namespace WavesVisuals
{
	/* PRIVATE CONSTANTS */
	const float Particle::MAX_TWIST = 0.025f;

	/* CONSTRUCTORS */
	Particle::Particle(void)
	{
		enabled = false;
		position = Point3f(0.0f, 0.0f, 0.0f);
		scale = 1.0f;
		trailLength = DEFAULT_TRAIL_LENGTH;
		
		float random = (float)rand() / (float)RAND_MAX;
		twist = (random * (MAX_TWIST * 0.33f)) + (MAX_TWIST * 0.77f);

		trail = new TrailPoint *[trailLength];
		for(unsigned int i = 0; i < trailLength; i++)
		{
			TrailPoint *t = new TrailPoint;
			assert(t != NULL);

			t->enabled = false;
			t->pos = new Point3f(0.0f, 0.0f, 0.0f);
			trail[i] = t;
		}

		trailPtr = 0;
	}

	Particle::~Particle(void)
	{
		if(trail != NULL)
		{
			delete [] trail;
			trail = NULL;
		}
	}

	/* PUBLIC MEMBER FUNCTIONS */
	const Vector3f &Particle::getDirection(void) const
	{
		return direction;
	}

	Point3f &Particle::getPosition(void)
	{
		return position;
	}

	float Particle::getScale(void) const
	{
		return scale;
	}
	
	Particle::TrailPoint **Particle::getTrail(void) const
	{
		return trail;
	}

	Particle::TrailPoint **Particle::getTrail(unsigned int &trailLength, unsigned int &trailPtr) const
	{
		trailLength = this->trailLength;
		trailPtr = this->trailPtr;
		return trail;
	}

	unsigned int Particle::getTrailLength(void) const
	{
		return trailLength;
	}

	unsigned int Particle::getTrailPtr(void) const
	{
		return trailPtr;
	}

	float Particle::getTwist(void) const
	{
		return twist;
	}

	bool Particle::isEnabled(void) const
	{
		return enabled;
	}

	void Particle::resetTrail(void)
	{
		for(unsigned int i = 0; i < trailLength; i++)
		{
			TrailPoint *tp = trail[i];
			assert(tp != NULL);

			tp->enabled = false;
		}
	}

	void Particle::saveTrailPosition(void)
	{
		float px = position.getX();
		float py = position.getY();
		float pz = position.getZ();

		TrailPoint *tp = trail[trailPtr];
		assert(tp != NULL);

		tp->pos->setX(px);
		tp->pos->setY(py);
		tp->pos->setZ(pz);
		tp->enabled = true;

		trailPtr = (++trailPtr % trailLength);
	}

	void Particle::setEnabled(bool enabled)
	{
		this->enabled = enabled;
	}

	void Particle::setDirection(const Vector3f &direction)
	{
		this->direction = direction;
	}

	void Particle::setPosition(const Point3f &position)
	{
		this->position = position;
	}

	void Particle::setScale(float scale)
	{
		this->scale = scale;
	}
}
