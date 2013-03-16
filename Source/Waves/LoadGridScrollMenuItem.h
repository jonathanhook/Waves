/**
 * Class:	LoadGridScrollMenu
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */
#ifndef LoadGridScrollMenuItem_H
#define LoadGridScrollMenuItem_H

#include "FastDelegate.h"
#include "GridScrollMenuItem.h"

using namespace fastdelegate;

namespace Waves
{
	class VisualWavesControl;

	class LoadGridScrollMenuItem :
		public GridScrollMenuItem
	{
	public:
		typedef FastDelegate1<VisualWavesControl *> SavedControlSelectedCallback;

		LoadGridScrollMenuItem(const Point2f &position, float width, float height, VisualWavesControl *control);
		~LoadGridScrollMenuItem(void);

		void fingerAdded(const FingerEventArgs &e);
		void fingerUpdated(const FingerEventArgs &e);
		void fingerRemoved(const FingerEventArgs &e);
		const VisualWavesControl &getControl(void) const;
		void render(void);
		void setGrayed(bool grayed);
		void setSavedControlSelectedCallback(SavedControlSelectedCallback savedControlSelected);

	private:
		VisualWavesControl *control;
		bool grayed;
		SavedControlSelectedCallback savedControlSelected;

		void control_Closed(VisualWavesControl *control);
	};
}
#endif

