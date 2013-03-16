/**
 * Class:	ToggleButton
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */
#include "RadButton.h"
#include "RadButtonGroup.h"

namespace Waves
{
	/* CONSTRUCTORS */
	RadButton::RadButton(const Point2f &position, float width, float height, RadButtonGroup *radButtonGroup, bool toggled) :
		ToggleButton(position, width, height, toggled)
	{
		this->radButtonGroup = radButtonGroup;

		assert(radButtonGroup != NULL);
		radButtonGroup->join(this);
	}

	RadButton::~RadButton(void)
	{
	}

	/* PUBLIC MEMBER FUNCTIONS */
	void RadButton::fingerRemoved(const FingerEventArgs &e)
	{
		Button::fingerRemoved(e);

		assert(radButtonGroup != NULL);

		if(contains(e) && !toggled)
		{
			toggled = true;
			radButtonGroup->unToggleOthers(this);

			if(toggleChanged != NULL) toggleChanged(toggled);
		}

		selected = toggled;
	}

	void RadButton::unToggle(void)
	{
		selected = false;
		toggled = false;
		dirty = true;
		if(toggleChanged != NULL) toggleChanged(toggled);
	}

}
