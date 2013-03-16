/**
 * Class:	ToggleButton
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */
#ifndef ToggleButton_H
#define ToggleButton_H

#include "Button.h"
#include "FastDelegate.h"
#include "JDHUtility/Point2f.h"

using namespace fastdelegate;

namespace Waves
{
	class ToggleButton :
		public Button
	{
	public:
		typedef FastDelegate1<bool> ToggleChangedCallback;

		ToggleButton(const Point2f &position, float width, float height, bool toggled = false);
		~ToggleButton(void);

		void fingerRemoved(const FingerEventArgs &e);
		bool isToggled(void);
		void setToggleChangedCallback(ToggleChangedCallback toggleChanged);
		void setToggled(bool toggled);

	protected:
		ToggleChangedCallback toggleChanged;

		bool toggled;
	};
}
#endif

