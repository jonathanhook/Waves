/**
 * Class:	Stars
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */
#ifndef Stars_H
#define Stars_H

#include <string>
#include "ParticleSystemVisual.h"

namespace WavesVisuals
{
	class Stars : 
		public ParticleSystemVisual
	{
	public:
		Stars(void);
		~Stars(void);

		Visual *getInstance(void) const;

	private:
		static const std::string	JITTER_NAME;
		static const float			MAX_JITTER;
		static const float			MAX_Z_TRANSLATE;
		static const std::string	PARTICLE_DISPLAY_LIST_KEY;
		static const std::string	SPEED_NAME;
		static const std::string	STAR_TEXTURE_PATH;
		static const std::string	STAR_TRAIL_TEXTURE_PATH;
		static const float			STAR_DIM;
		static const std::string	TRAIL_DISPLAY_LIST_KEY;
		static const std::string	TRAIL_LENGTH_NAME;
		static const float			Z_START_POS;

		std::string particleDisplayListKey;
		std::string trailDisplayListKey;
		bool trailDirty;

		void renderParticle(Particle *p);
		void renderTrail(Particle *p);
		void resetParticle(Particle *p);
		void updateParticle(Particle *p);

	};
}
#endif

