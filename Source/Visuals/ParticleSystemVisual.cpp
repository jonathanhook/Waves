/**
 * Class:	ParticleSystemVisual
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */
#include "JDHUtility/GlutInclude.h"
#include "Waves/InterfaceManager.h"
#include "Particle.h"
#include "ParticleSystemVisual.h"

using namespace Waves;

namespace WavesVisuals
{
	/* PRIVATE CONSTANTS */
	const float ParticleSystemVisual::MAX_CHANCE = 0.25f;

	/* PROTECTED CONSTANTS */
	const std::string ParticleSystemVisual::INTENSITY_NAME	= "Intensity";
	const std::string ParticleSystemVisual::TWIST_NAME		= "Twist";

	/* CONSTRUCTORS */
	ParticleSystemVisual::ParticleSystemVisual(std::string name, unsigned int numParticles, bool combinedScale) :
		Visual(name, combinedScale)
	{
		this->numParticles = numParticles;

		particles = new Particle*[numParticles];
		for(unsigned int i = 0; i < numParticles; i++)
		{
			Particle *p = new Particle();
			particles[i] = p;
		}

		particlePointer = 0;

		// new parameters
		parameters[INTENSITY_NAME] = new Parameter(INTENSITY_NAME);
		parameters[TWIST_NAME] = new Parameter(TWIST_NAME, 0.0f);
	}

	ParticleSystemVisual::~ParticleSystemVisual(void)
	{
		if(particles != NULL) delete [] particles;
		particles = NULL;
	}

	/* PUBLIC MEMBER FUNCTIONS */
	void ParticleSystemVisual::draw(void)
	{
		for(unsigned int i = 0; i < numParticles; i++)
		{
			Particle *p = particles[i];
			assert(p != NULL);

			if(p->isEnabled())
			{
				renderParticle(p);
				renderTrail(p);
			}
		}
	}

	void ParticleSystemVisual::update(void)
	{
		unsigned int now = glutGet(GLUT_ELAPSED_TIME);
		unsigned int elapsed = (now - last);

		if(elapsed > UPDATE_RATE_MS)
		{
			last = now;

			// update
			for(unsigned int i = 0; i < numParticles; i++)
			{
				Particle *p = particles[i];
				assert(p != NULL);

				if(p->isEnabled())
				{
					updateParticle(p);
					p->saveTrailPosition();

					const Point3f &pos = p->getPosition();
					if(pos.getZ() >= InterfaceManager::CAMERA_Z)
					{
						p->setEnabled(false);
					}
				}
			}

			// make a new one?
			Parameter *param = parameters[INTENSITY_NAME];
			assert(param != NULL);

			float intensity = param->getValue();
			float chance = intensity * MAX_CHANCE;
			float random = getRandom();

			if(random > (1.0f - chance))
			{
				newParticle();
			}
		}
	}

	/* PROTECTED MEMBER FUNCTIONS */
	void ParticleSystemVisual::newParticle(void)
	{
		Particle *p = particles[particlePointer];
		resetParticle(p);

		particlePointer = (particlePointer + 1) % numParticles;
	}
}
