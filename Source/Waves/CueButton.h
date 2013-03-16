/**
 * Class:	CueButton
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */
#ifndef CueButton_H
#define CueButton_H

#include "ToggleButton.h"

namespace Waves
{
	class CueButton :
		public ToggleButton
	{
	public:
		CueButton(const Point2f &position, float width, float height, bool toggled = false);
		~CueButton(void);

		void render(void);

	private:
		bool cueButtonDirty;
		unsigned int cueButtonDisplayList;
	};
}
#endif

