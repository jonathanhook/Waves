/**
 * Class:	ToggleButton
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */
#ifndef RadButton_H
#define RadButton_H

#include "ToggleButton.h"

namespace Waves
{
	class RadButtonGroup;

	class RadButton :
		public ToggleButton
	{
	public:
		RadButton(const Point2f &position, float width, float height, RadButtonGroup *radButtonGroup, bool toggled = false);
		~RadButton(void);

		void fingerRemoved(const FingerEventArgs &e);
		void unToggle(void);

	private:
		RadButtonGroup *radButtonGroup;
	};
}
#endif

