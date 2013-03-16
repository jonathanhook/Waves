/**
 * Class:	Track
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */
#ifndef Track_H
#define Track_H

#include <vector>
#include "FastDelegate.h"
#include "FFTSplineControl.h"
#include "InterfaceElement.h"
#include "SplinePreset.h"
#include "TemporalSplineControl.h"

namespace WavesVisuals { class Parameter; }

using namespace fastdelegate;
using namespace WavesVisuals;

namespace Waves
{
	class PresetMenu;
	class TrackMenu;

	class Track :
		public InterfaceElement
	{
	public:
		typedef enum
		{
			FFT,
			TEMPORAL
		} TrackMode;

		typedef FastDelegate1<Track *> ExitedCallback;

		Track(const Point2f &position, float width, float height, Parameter *parameter);
		~Track(void);

		void addFFTPreset(unsigned int pId);
		void addTemporalPreset(unsigned int pId);
		bool contains(const FingerEventArgs &e) const;
		void fingerAdded(const FingerEventArgs &e);
		void fingerUpdated(const FingerEventArgs &e);
		void fingerRemoved(const FingerEventArgs &e);
		const std::vector<unsigned int> &getFFTPresets(void) const;
		FFTSplineControl &getFFTSplineControl(void);
		const TrackMode &getMode(void) const;
		const Parameter &getParameter(void) const;
		const std::vector<unsigned int> &getTemporalPresets(void) const;
		TemporalSplineControl &getTemporalSplineControl(void);
		float getValue(void) const;
		void render(void);
		void renderSpline(void);
		void setExitedCallback(ExitedCallback exited);
		void setMode(TrackMode mode);

	private:
		static const TrackMode DEFAULT_TRACK_MODE = TEMPORAL;
		static const float TRACK_MENU_DIM;

		ExitedCallback exited;
		std::vector<unsigned int> fftPresets;
		FFTSplineControl *fftSplineControl;
		TrackMenu *menu;
		TrackMode mode;
		Parameter *parameter;
		PresetMenu *presetMenu;
		bool presetMenuDisplayed;
		std::vector<unsigned int> temporalPresets;
		TemporalSplineControl *temporalSplineControl;

		void initInterfaceElements(void);
		void PresetMenu_ItemSelected(SplinePreset *preset);
		void sliderButton_Clicked(void);
		void spline_Exited(void);
		void trackMenu_FFTSelected(void);
		void trackMenu_PresetMenuToggled(bool selected);
		void trackMenu_Reset(void);
		void trackMenu_Saved(void);
		void trackMenu_TemporalSelected(void);
	};
}
#endif

