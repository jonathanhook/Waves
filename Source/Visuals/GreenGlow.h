/**
 * Class:	GreenGlow
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */
#ifndef GreenGlow_H
#define GreenGlow_H

#include "Visuals/BackgroundImg.h"

namespace WavesVisuals
{
	class GreenGlow :
		public BackgroundImg
	{
	public:
		GreenGlow(void);
		~GreenGlow(void);

		Visual *getInstance(void) const;

	private:
		static const std::string IMG_PATH;
	};
}
#endif