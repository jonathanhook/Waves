/**
 * Class:	NewGridScrollMenu
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */
#ifndef NewGridScrollMenu_H
#define NewGridScrollMenu_H

#include "FastDelegate.h"
#include "GridScrollMenu.h"

using namespace fastdelegate;

namespace Waves
{
	class NewGridScrollMenu : 
		public GridScrollMenu
	{
	public:
		typedef FastDelegate1<Visual *> VisualSelectedCallback;

		NewGridScrollMenu(const Point2f &position, float width, float height, std::vector<Visual *> visuals);
		~NewGridScrollMenu(void);

		void setVisualSelectedCallback(VisualSelectedCallback visualSelected);

	protected:
		void initInterfaceElements(void);

	private:
		std::vector<Visual *> visuals;
		VisualSelectedCallback visualSelected;

		void gridScrollMenuItem_VisualSelected(Visual *visual);
	};
}
#endif