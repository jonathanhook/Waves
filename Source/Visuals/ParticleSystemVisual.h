/**
 * Class:	ParticleSystemVisual
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */
#ifndef ParticleSystemVisual_H
#define ParticleSystemVisual_H

#include <string>
#include "Visuals/Visual.h"

namespace WavesVisuals
{
	class Particle;

	class ParticleSystemVisual :
		public Visual
	{
	public:
		ParticleSystemVisual(std::string name, unsigned int numParticles = DEFAULT_NUM_PARTICLES, bool combinedScale = true);
		~ParticleSystemVisual(void);

		void draw(void);
		void update(void);

	protected:
		static const unsigned int DEFAULT_NUM_PARTICLES = 150;
		static const std::string INTENSITY_NAME;
		static const std::string TWIST_NAME;

		unsigned int numParticles;
		Particle **particles;

		void newParticle(void);
		virtual void renderParticle(Particle *p) = 0;
		virtual void renderTrail(Particle *p) = 0;
		virtual void resetParticle(Particle *p) = 0;
		virtual void updateParticle(Particle *p) = 0;

	private:
		static const float MAX_CHANCE;
		static const unsigned int UPDATE_RATE_MS = 28;

		unsigned int last;
		unsigned int particlePointer;
	};
}
#endif

