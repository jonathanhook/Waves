/**
 * Class:	TextScrollMenu
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */
#ifndef TextScrollMenu_H
#define TextScrollMenu_H

#include <list>
#include <vector>
#include "FastDelegate.h"
#include "JDHUtility/Point2f.h"
#include "ScrollMenu.h"

using namespace fastdelegate;
using namespace JDHUtility;

namespace Waves
{
	class TextScrollMenuItem;

	class TextScrollMenu :
		public ScrollMenu
	{
	public:
		typedef FastDelegate1<std::string> ItemSelectedCallback;

		TextScrollMenu(const Point2f &position, float preferredHeight, std::vector<std::string> menuItems);
		~TextScrollMenu(void);

		void fingerRemoved(const FingerEventArgs &e);
		void render(void);
		void setItemSelected(ItemSelectedCallback itemSelected);

	private:
		static const float CHAR_SCALE;

		ItemSelectedCallback itemSelected;
		std::vector<std::string> menuItems;

		void renderMenuItem(std::string text, bool selected);
		void renderScrollBar(void);
	};
}
#endif

