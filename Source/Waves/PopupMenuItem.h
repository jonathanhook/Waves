/**
 * Class:	PopupMenuItem
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */
#ifndef PopupMenuItem_H
#define PopupMenuItem_H

#include "Button.h"
#include "FastDelegate.h"
#include "Finger.h"

using namespace fastdelegate;

namespace Waves
{
	class PopupMenuItem :
		public Button
	{
	public:
		//typedef FastDelegate1<WaveVisual *> ClickedCallback;

		PopupMenuItem(const Point2f &position, float width, float height);
		~PopupMenuItem(void);

		void fingerRemoved(const FingerEventArgs &e);
		void render(void);
		void setClickedCallback(ClickedCallback clicked);

	private:
		static float SELECTED_OPACITY;
		static float TEXTURE_OPACITY;

		//ClickedCallback clicked;
		unsigned int popupMenuItemDisplayList;
		//WaveVisual *visual;
	};
}
#endif
