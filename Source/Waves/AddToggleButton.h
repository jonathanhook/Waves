/**
 * Class:	AddToggleButton
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */
#ifndef AddToggleButton_H
#define AddToggleButton_H

#include "ToggleButton.h"
#include "JDHUtility/Point2f.h"

using namespace JDHUtility;

namespace Waves
{
	class AddToggleButton :
		public ToggleButton
	{
	public:
		AddToggleButton(const Point2f &position, float width, float height, bool toggled = false);
		~AddToggleButton(void);

		void render(void);

	private:
		bool addToggleButtonDirty;
		unsigned int addToggleButtonDisplayList;
	};
}
#endif

