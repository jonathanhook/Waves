/**
 * Class:	TrackMenu
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */
#ifndef TrackMenu_H
#define TrackMenu_H

#include <string>
#include "InterfaceElement.h"
#include "Track.h"

namespace Waves
{
	class FFTButton;
	class SplineButton;
	class SplineButton;
	class LoadButton;

	class TrackMenu :
		public InterfaceElement
	{
	public:
		typedef FastDelegate0<> FFTSelectedCallback;
		typedef FastDelegate1<bool> PresetMenuToggledCallback;
		typedef FastDelegate0<> ResetCallback;
		typedef FastDelegate0<> SavedCallback;
		typedef FastDelegate0<> TemporalSelectedCallback;
		
		TrackMenu(const Point2f &p, float width, float height, std::string title);
		~TrackMenu(void);

		void deSelectLoadButton(void);
		void fingerAdded(const FingerEventArgs &e);
		void fingerUpdated(const FingerEventArgs &e);
		void fingerRemoved(const FingerEventArgs &e);
		void render(void);
		void selectFFTMode(void);
		void selectTemporalMode(void);
		void setFFTSelectedCallback(FFTSelectedCallback fftSelected);
		void setPresetMenuToggledCallback(PresetMenuToggledCallback presetMenuToggled);
		void setResetCallback(ResetCallback reset);
		void setSavedCallback(SavedCallback saved);
		void setTemporalSelectedCallback(TemporalSelectedCallback temporalSelected);

	private:
		FFTButton *fftModeButton;
		FFTSelectedCallback fftSelected;
		LoadButton *loadButton;
		PresetMenuToggledCallback presetMenuToggled;
		ResetCallback reset;
		SavedCallback saved;
		SplineButton *temporalModeButton;
		TemporalSelectedCallback temporalSelected;
		std::string title;

		void fftModeButton_Clicked(void);
		void initInterfaceElements(void);
		void loadButton_ToggleChanged(bool selected);
		void resetButton_Clicked(void);
		void saveButton_Clicked(void);
		void temporalModeButton_Clicked(void);
	};
}
#endif
