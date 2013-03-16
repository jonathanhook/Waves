/**
 * Class:	PurpleGradient
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */
#ifndef PurpleGradient_H
#define PurpleGradient_H

#include "Visuals/BackgroundImg.h"

namespace WavesVisuals
{
	class PurpleGradient :
		public BackgroundImg
	{
	public:
		PurpleGradient(void);
		~PurpleGradient(void);

		Visual *getInstance(void) const;

	private:
		static const std::string IMG_PATH;
	};
}
#endif