/**
 * Class:	WavesGlobalState
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */
#include <stdio.h>
#include "Visuals/BlueGradient.h"
#include "Visuals/Curves.h"
#include "JDHUtility/FPSMonitor.h"
#include "JDHUtility/GlutInclude.h"
#include "Visuals/GreenGlow.h"
#include "InterfaceManager.h"
#include "FFT/LiveFFT.h"
#include "Playhead.h"
#include "Visuals/PurpleGradient.h"
#include "Visuals/RedGlow.h"
#include "Visuals/Rectangles.h"
#include "SplinePreset.h"
#include "Visuals/Stars.h"
#include "Visuals/Terrain.h"
#include "VisualWavesControl.h"
#include "WavesGlobalState.h"

namespace Waves
{
	/* PRIVATE CONSTANS */
	const float WavesGlobalState::DEFAULT_BPM = 120.0f;
	const char *WavesGlobalState::externalIpAddress;
	/* PUBLIC STATIC FUNCTIONS */
	WavesGlobalState *WavesGlobalState::getInstance(void)
	{
		if(instance == NULL)
		{
			instance = new WavesGlobalState();
		}

		return (WavesGlobalState *)instance;
	}

	/* CONSTRUCTORS */
	WavesGlobalState::WavesGlobalState(void) :
		Singleton()
	{
		bpm = DEFAULT_BPM;
		last = glutGet(GLUT_ELAPSED_TIME);
		
		fft = new LiveFFT();
		fpsMonitor = new FPSMonitor();
		interfaceElementCount = 0;
		playhead = new Playhead();

		float *lmColour = new float[3];
		lmColour[0] = 1.0f;
		lmColour[1] = 1.0f;
		lmColour[2] = 1.0f;

		float *lmEyePos = new float[3];
		lmEyePos[0] = 0.0f;
		lmEyePos[1] = 0.0f;
		lmEyePos[2] = -1.0f;

		float *lmLightPos = new float[3];
		lmLightPos[0] = 0.0f;
		lmLightPos[1] = 0.0f;
		lmLightPos[2] = 1.0f;

		fft->start();

		initFactoryVisuals();
	}

	WavesGlobalState::~WavesGlobalState(void)
	{
		if(fpsMonitor != NULL) delete fpsMonitor;
		fpsMonitor = NULL;

		if(playhead != NULL) delete playhead;
		playhead = NULL;

		for(std::map<unsigned int, SplinePreset *>::iterator it = splinePresets.begin(); it != splinePresets.end(); it++)
		{
			if((*it).second != NULL) delete (*it).second;
			(*it).second = NULL;
		}
		splinePresets.clear();
	}

	/* PUBLIC MEMBER FUNCTIONS */
	void WavesGlobalState::addSavedControl(VisualWavesControl *control)
	{
		assert(control != NULL);

		unsigned int controlId = control->getId();
		if(savedControls.count(controlId) == 0)
		{
			savedControls[controlId] = control;
		}
	}
	
	void WavesGlobalState::decrementInterfaceElementCount(void)
	{
		interfaceElementCount--;
/*
#ifdef _DEBUG
		printf("Num InterfaceElements: %d\r\n", interfaceElementCount);
#endif*/
	}

	float WavesGlobalState::getBpm(void) const
	{
		return bpm;
	}

	std::map<unsigned int, SplinePreset *> &WavesGlobalState::getDefaultPresets(void)
	{
		return defaultPresets;
	}

	const std::vector<Visual *> &WavesGlobalState::getFactoryVisuals(void) const
	{
		return factoryVisuals;
	}

	const LiveFFT &WavesGlobalState::getFFT(void) const
	{
		return *fft;
	}

	FPSMonitor &WavesGlobalState::getFPSMonitor(void)
	{
		return *fpsMonitor;
	}

	unsigned int WavesGlobalState::getInterfaceElementCount(void) const
	{
		return interfaceElementCount;
	}

	Playhead &WavesGlobalState::getPlayhead(void)
	{
		return *playhead;
	}

	const std::map<unsigned int, VisualWavesControl *> &WavesGlobalState::getSavedControls(void) const
	{
		return savedControls;
	}

	std::map<unsigned int, SplinePreset *> &WavesGlobalState::getSplinePresets(void)
	{
		return splinePresets;
	}

	void WavesGlobalState::initFactoryVisuals(void)
	{
		//factoryVisuals.push_back(new Wash());
		factoryVisuals.push_back(new Stars());
		//factoryVisuals.push_back(new DarkGlow());
		factoryVisuals.push_back(new Curves());
		factoryVisuals.push_back(new Rectangles());
		//factoryVisuals.push_back(new Terrain());
		factoryVisuals.push_back(new BlueGradient);
		factoryVisuals.push_back(new PurpleGradient());
		factoryVisuals.push_back(new GreenGlow());
		factoryVisuals.push_back(new RedGlow());
	}

	void WavesGlobalState::incrementInterfaceElementCount(void)
	{
		interfaceElementCount++;
		/*
#ifdef _DEBUG
		printf("Num InterfaceElements: %d\r\n", interfaceElementCount);
#endif
		*/
	}

	void WavesGlobalState::loadContent(void)
	{
		loadPresets();
		loadWavesControls();
	}

	void WavesGlobalState::setBpm(float bpm)
	{
		this->bpm = bpm;
	}

	void WavesGlobalState::updatePlayheadPosition(void)
	{
		assert(playhead != NULL);

		int now = glutGet(GLUT_ELAPSED_TIME);
		int elapsed = now - last;
		last = now;

		if(!playhead->isHeld())
		{
			int loopTime = (60000 / bpm) * InterfaceManager::BEATS;
		
			float playheadPosition = playhead->getPosition() + (float)elapsed / (float)loopTime;
		
			if(playheadPosition > 1.0f)
			{
				playheadPosition = playheadPosition - 1.0f;
			}

			playhead->setPlayheadPosition(playheadPosition);
		}
	}

	/* PRIVATE MEMBER FUNCTIONS */
	void WavesGlobalState::loadPresets(void)
	{
		defaultPresets = SplinePreset::loadDefaults();
		splinePresets = SplinePreset::loadAll();
	}

	void WavesGlobalState::loadWavesControls(void)
	{
		/*
		std::map<unsigned int, VisualWavesControl *> loadedControls = VisualWavesControl::loadAll();




		std::map<unsigned int, VisualWavesControl *>::iterator it; 
		for(it = loadedControls.begin(); it != loadedControls.end(); it++)
		{
			VisualWavesControl *v = (*it).second;
			assert(v != NULL);

			unsigned int vId = v->getId();
			savedControls[vId] = v;
		}*/
	}
}
