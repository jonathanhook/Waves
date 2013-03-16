/**
 * Class:	WavesGlobalState
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */
#ifndef WavesGlobalState_H
#define WavesGlobalState_H

#include "JDHUtility/Singleton.h"

namespace JDHUtility { class FPSMonitor; }
namespace FFT { class LiveFFT; }
namespace WavesVisuals { class Visual; }

using namespace JDHUtility;
using namespace FFT;
using namespace WavesVisuals;

namespace Waves
{
	class Playhead;
	class SplinePreset;
	class VisualWavesControl;

	class WavesGlobalState :
		public Singleton
	{
	public:
		static const char *externalIpAddress;
		static WavesGlobalState *getInstance(void);

		void addSavedControl(VisualWavesControl *control);
		void decrementInterfaceElementCount(void);
		float getBpm(void) const;
		std::map<unsigned int, SplinePreset *> &getDefaultPresets(void);
		const LiveFFT &getFFT(void) const;
		const std::vector<Visual *> &getFactoryVisuals(void) const;
		FPSMonitor &getFPSMonitor(void);
		unsigned int getInterfaceElementCount(void) const;
		Playhead &getPlayhead(void);
		const std::map<unsigned int, VisualWavesControl *> &getSavedControls(void) const;
		std::map<unsigned int, SplinePreset *> &getSplinePresets(void);
		void incrementInterfaceElementCount(void);
		void loadContent(void);
		void setBpm(float bpm);
		void updatePlayheadPosition(void);

	private:
		static const float DEFAULT_BPM;

		WavesGlobalState::WavesGlobalState(void);
		WavesGlobalState::~WavesGlobalState(void);

		float bpm;
		std::map<unsigned int, SplinePreset *> defaultPresets;
		std::vector<Visual *> factoryVisuals;
		LiveFFT *fft;
		FPSMonitor *fpsMonitor;
		void initFactoryVisuals(void);
		unsigned int interfaceElementCount;
		unsigned int last;
		Playhead *playhead;
		std::map<unsigned int, VisualWavesControl *> savedControls;
		std::map<unsigned int, SplinePreset *> splinePresets;

		void loadPresets(void);
		void loadWavesControls(void);
	};
}
#endif

