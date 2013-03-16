/**
 * Class:	PopupMenu
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */
#ifndef PopupMenu_H
#define PopupMenu_H

#include <vector>
#include "FastDelegate.h"
#include "JDHUtility/Point2f.h"
#include "JDHUtility/Vector2f.h"
#include "InterfaceElement.h"

namespace WavesVisuals { class Visual; }

using namespace fastdelegate;
using namespace WavesVisuals;

namespace Waves
{
	class BPMControl;
	class LoadGridScrollMenu;
	class NewGridScrollMenu;
	class ExternalVisualScrollMenu;
	class RadButton;
	class VisualWavesControl;

	class PopupMenu :
		public InterfaceElement
	{
	public:
		typedef FastDelegate0<> ExitedCallback;
		typedef FastDelegate1<VisualWavesControl *> SavedControlSelectedCallback;
		typedef FastDelegate1<Visual *> VisualSelectedCallback;

		PopupMenu(const Point2f &position, float width, float height);
		~PopupMenu(void);

		void fingerAdded(const FingerEventArgs &e);
		void fingerUpdated(const FingerEventArgs &e);
		void fingerRemoved(const FingerEventArgs &e);
		void render(void);
		void setExitedCallback(ExitedCallback exited);
		void setGrayed(VisualWavesControl *control, bool grayed);
		void setSavedControlSelectedCallback(SavedControlSelectedCallback savedControlSelected);
		void setVisualSelectedCallback(VisualSelectedCallback visualSelected);

	private:
		typedef enum
		{
			NEW,
			LOAD,
			BPM,
			EXVIS
		} MenuState;

		static float BACKGROUND_OPACITY;
		static float BORDER_THICKNESS;
		static float EXIT_BUTTON_DIM;
		static int MAX_DIM;

		BPMControl *bpmControl;
		ExitedCallback exited;
		LoadGridScrollMenu *loadGrid;
		MenuState menuState;
		NewGridScrollMenu *newGrid;
		ExternalVisualScrollMenu *exVisGrid;
		SavedControlSelectedCallback savedControlSelected;
		VisualSelectedCallback visualSelected;

		void bpmTab_Clicked(void);
		void exitButton_Clicked(void);
		void initInterfaceElements(void);
		void loadGrid_SavedControlSelected(VisualWavesControl *control);
		void loadTab_Clicked(void);
		void newTab_Clicked(void);
		void newGrid_VisualSelected(Visual *visual);
		void topBar_Dragged(const Vector2f &v);

		void exVis_Clicked(void);
		void exVis_VisualSelected(Visual *visual);
	};
}
#endif
