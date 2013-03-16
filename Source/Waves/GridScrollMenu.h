/**
 * Class:	GridScrollMenu
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */
#ifndef GridScrollMenu_H
#define GridScrollMenu_H

#include <vector>
#include "FastDelegate.h"
#include "InterfaceElement.h"

namespace WavesVisuals { class Visual; }

using namespace fastdelegate;
using namespace WavesVisuals;

namespace Waves
{
	class GridScrollMenu : 
		public InterfaceElement
	{
	public:
		GridScrollMenu(const Point2f &position, float width, float height);
		~GridScrollMenu(void);

		bool contains(const FingerEventArgs &e) const;
		void fingerAdded(const FingerEventArgs &e);
		void fingerUpdated(const FingerEventArgs &e);
		virtual void fingerRemoved(const FingerEventArgs &e);
		void render(void);

	protected:
		static const unsigned int GRID_DIM = 3;
		static const float SCROLL_BAR_BORDER;
		
		float itemHeight;
		float itemWidth;

		virtual void initInterfaceElements(void) = 0;

	private:

		void renderScrollBar(void);
	};
}
#endif

