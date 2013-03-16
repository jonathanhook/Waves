/**
 * Class:	LoadGridScrollMenu
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */
#ifndef LoadGridScrollMenu_H
#define LoadGridScrollMenu_H

#include "FastDelegate.h"
#include "GridScrollMenu.h"

using namespace fastdelegate;

namespace Waves
{
	class VisualWavesControl;

	class LoadGridScrollMenu : 
		public GridScrollMenu
	{
	public:
		typedef FastDelegate1<VisualWavesControl *> SavedControlSelectedCallback;

		LoadGridScrollMenu(const Point2f &position, float width, float height);
		~LoadGridScrollMenu(void);

		void addItem(unsigned int id);		
		void setGrayed(VisualWavesControl *control, bool grayed);
		void setSavedControlSelectedCallback(SavedControlSelectedCallback savedControlSelected);
		void update(void);

	protected:
		void initInterfaceElements(void);

	private:
		unsigned int currentSize;
		SavedControlSelectedCallback savedControlSelected;

		void loadGridScrollMenuItem_savedControlSelected(VisualWavesControl *control);
	};
}
#endif

