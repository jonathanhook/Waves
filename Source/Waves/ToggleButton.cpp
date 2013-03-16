/**
 * Class:	ToggleButton
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */
#include "ToggleButton.h"

namespace Waves
{
	/* CONSTRUCTORS */
	ToggleButton::ToggleButton(const Point2f &position, float width, float height, bool toggled) :
		Button(position, width, height)
	{
		this->toggled = toggled;
		selected = toggled;
	}

	ToggleButton::~ToggleButton(void)
	{
	}

	/* PUBLIC MEMBER FUNCTIONS */
	void ToggleButton::fingerRemoved(const FingerEventArgs &e)
	{
		Button::fingerRemoved(e);

		if(contains(e))
		{
			toggled = !toggled;
			selected = toggled;

			if(toggleChanged != NULL) toggleChanged(toggled);
		}
	}

	bool ToggleButton::isToggled(void)
	{
		return toggled;
	}

	void ToggleButton::setToggleChangedCallback(ToggleChangedCallback toggleChangedCallback)
	{
		this->toggleChanged = toggleChangedCallback;
	}

	void ToggleButton::setToggled(bool toggled)
	{
		this->toggled = toggled;
		selected = toggled;
		dirty = true;
	}
}
