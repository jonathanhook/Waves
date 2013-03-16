/**
 * Class:	MainControlPanel
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */
#ifndef MainControlPanel_H
#define MainControlPanel_H

#include "InterfaceElement.h"

namespace WavesVisuals { class Visual; }

using namespace WavesVisuals;

namespace Waves
{
	class MainControlPanel :
		public InterfaceElement
	{
	public:
		typedef FastDelegate0<> ClosedCallback;
		typedef FastDelegate1<bool> CueChangedCallback;
		typedef FastDelegate0<> SaveCallback;

		MainControlPanel(const Point2f &position, float width, float height, Visual *visual, bool forOutput = false);
		~MainControlPanel(void);

		bool contains(const FingerEventArgs &e) const;
		void fingerAdded(const FingerEventArgs &e);
		void fingerUpdated(const FingerEventArgs &e);
		void fingerRemoved(const FingerEventArgs &e);
		void render(void);
		void setClosedCallback(ClosedCallback closed);
		void setCueChangedCallback(CueChangedCallback cueChanged);
		void setSaveCallback(SaveCallback save);

	private:
		ClosedCallback closed;
		CueChangedCallback cueChanged;
		bool forOutput;
		SaveCallback save;
		Visual *visual;

		void cueButton_ToggleChanged(bool selected);
		void exitButton_Clicked(void);
		void initInterfaceElements(void);
		void saveButton_Clicked(void);
		void saveAsButton_Clicked(void);
	};
}
#endif

