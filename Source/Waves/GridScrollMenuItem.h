/**
 * Class:	GridScrollMenuItem
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */
#ifndef GridScrollMenuItem_H
#define GridScrollMenuItem_H

#include "Button.h"
#include "FastDelegate.h"

namespace WavesVisuals { class Visual; }

using namespace fastdelegate;
using namespace WavesVisuals;

namespace Waves
{
	class GridScrollMenuItem :
		public Button
	{
	public:
		typedef FastDelegate1<Visual *> VisualSelectedCallback;

		GridScrollMenuItem(const Point2f &position, float width, float height, Visual *visual);
		~GridScrollMenuItem(void);

		void fingerRemoved(const FingerEventArgs &e);
		void render(void);
		void setVisualSelected(VisualSelectedCallback visualSelected);
		void update(void);

	protected:
		std::string textLabel;

	private:
		static const float TEXT_DIM;
		
		bool GridScrollMenuItemDirty;
		unsigned int GridScrollMenuItemDisplayList;
		Visual *visual;
		VisualSelectedCallback visualSelected;
	};
}
#endif

