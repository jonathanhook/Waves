/**
 * Class:	ScrollMenu
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */
#ifndef ScrollMenu_H
#define ScrollMenu_H

#include "FastDelegate.h"
#include "InterfaceElement.h"

using namespace fastdelegate;

namespace Waves
{
	class ScrollMenu :
		public InterfaceElement
	{
	public:
		typedef FastDelegate0<> TouchedCallback;

		typedef enum
		{
			VERTICAL,
			HORIZONTAL
		} ScrollDirection;

		ScrollMenu(const Point2f &position, float width, float height, ScrollDirection scrollDirection, unsigned int itemCount);
		~ScrollMenu(void);

		void fingerAdded(const FingerEventArgs &e);
		void fingerUpdated(const FingerEventArgs &e);
		virtual void fingerRemoved(const FingerEventArgs &e);
		void setTouchedCallback(TouchedCallback touched);
		virtual void render(void) = 0;
		void update(void);
	
	protected:
		static const unsigned int DISPLAY_COUNT = 6;
		static const float SCROLL_BAR_BORDER;

		float dAxis;
		unsigned int displayCount;
		Point2f down;
		bool fingerDown;
		int index;
		unsigned int itemCount;
		float itemDim;
		bool moved;
		ScrollDirection scrollDirection;
		bool selected;
		unsigned int selectedIndex;
		TouchedCallback touched;

	};
}
#endif

