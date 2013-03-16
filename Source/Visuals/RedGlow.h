/**
 * Class:	RedGlow
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */
#ifndef RedGlow_H
#define RedGlow_H

#include "Visuals/BackgroundImg.h"

namespace WavesVisuals
{
	class RedGlow :
		public BackgroundImg
	{
	public:
		RedGlow(void);
		~RedGlow(void);

		Visual *getInstance(void) const;

	private:
		static const std::string IMG_PATH;
	};
}
#endif