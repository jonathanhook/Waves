/**
 * Class:	PresetMenu
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */
#ifndef PresetMenu_H
#define PresetMenu_H

#include <map>
#include "ScrollMenu.h"
#include "SplinePreset.h"

namespace Waves
{
	class PresetMenu :
		public ScrollMenu
	{
	public:
		typedef FastDelegate1<SplinePreset *> ItemSelectedCallback;

		PresetMenu(const Point2f &position, float height, std::vector<SplinePreset *> menuItems);
		~PresetMenu(void);

		void fingerRemoved(const FingerEventArgs &e);
		void render(void);
		void setItemSelectedCallback(ItemSelectedCallback itemSelected);
		void setMenuItems(std::vector<SplinePreset *> menuItems);

	private:
		static const unsigned int PRESET_DISPLAY_COUNT = 3;

		ItemSelectedCallback itemSelected;
		std::vector<SplinePreset *> menuItems;

		void renderMenuItem(SplinePreset *preset, bool selected);
		void renderScrollBar(void);
		float scaleY(float y);
		void updateDisplayCount(void);

	};
}
#endif

