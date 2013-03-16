/**
 * Class:	BlueGradient
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */
#ifndef BlueGradient_H
#define BlueGradient_H

#include "Visuals/BackgroundImg.h"

namespace WavesVisuals
{
	class BlueGradient :
		public BackgroundImg
	{
	public:
		BlueGradient(void);
		~BlueGradient(void);

		Visual *getInstance(void) const;

	private:
		static const std::string IMG_PATH;
	};
}
#endif