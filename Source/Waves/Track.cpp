/**
 * Class:	Track
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */
#include "Button.h"
#include "FastDelegate.h"
#include "FFTSplineControl.h"
#include "InterfaceManager.h"
#include "Visuals/Parameter.h"
#include "PresetMenu.h"
#include "Playhead.h"
#include "TemporalSplineControl.h"
#include "Track.h"
#include "TrackMenu.h"
#include "WavesGlobalState.h"
#include "WavePreset.h"

using namespace fastdelegate;
using namespace WavesVisuals;

namespace Waves
{
	/* PRIVATE CONSTANTS */
	const float Track::TRACK_MENU_DIM = 0.935f;

	/* CONSTRUCTORS */
	Track::Track(const Point2f &position, float width, float height, Parameter *parameter) :
		InterfaceElement(position, width, height)
	{
		this->parameter = parameter;

		exited = NULL;
		mode = DEFAULT_TRACK_MODE;
		presetMenuDisplayed = false;

		initInterfaceElements();
	}

	Track::~Track(void)
	{
	}

	/* PUBLIC MEMBER FUNCTIONS */
	void Track::addFFTPreset(unsigned int pId)
	{
		bool novel = true;
		for(unsigned int j = 0; j < fftPresets.size(); j++)
		{
			if(fftPresets[j] == pId)
			{
				novel = false;
				break;
			}		
		}

		if(novel)
		{
			fftPresets.push_back(pId);
		}
	}
	void Track::addTemporalPreset(unsigned int pId)
	{
		bool novel = true;
		for(unsigned int j = 0; j < temporalPresets.size(); j++)
		{
			if(temporalPresets[j] == pId)
			{
				novel = false;
				break;
			}	
		}

		if(novel)
		{
			temporalPresets.push_back(pId);
		}
	}

	bool Track::contains(const FingerEventArgs &e) const
	{
		return false;
	}
	
	void Track::fingerAdded(const FingerEventArgs &e)
	{

	}
		
	void Track::fingerUpdated(const FingerEventArgs &e)
	{

	}

	void Track::fingerRemoved(const FingerEventArgs &e)
	{

	}

	const std::vector<unsigned int> &Track::getFFTPresets(void) const
	{
		return fftPresets;
	}

	FFTSplineControl &Track::getFFTSplineControl(void)
	{
		return *fftSplineControl;
	}

	const Track::TrackMode &Track::getMode(void) const
	{
		return mode;
	}

	const Parameter &Track::getParameter(void) const
	{
		return *parameter;
	}

	const std::vector<unsigned int> &Track::getTemporalPresets(void) const
	{
		return temporalPresets;
	}

	TemporalSplineControl &Track::getTemporalSplineControl(void)
	{
		return *temporalSplineControl;
	}

	float Track::getValue(void) const
	{
		if(mode == TEMPORAL)
		{
			assert(temporalSplineControl != NULL);
			return temporalSplineControl->getValue(WavesGlobalState::getInstance()->getPlayhead().getPosition());
		}
		else
		{
			assert(fftSplineControl != NULL);
			return fftSplineControl->getFFTResult();
		}
	}

	void Track::render(void)
	{
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();

		glTranslatef(position.getX(), position.getY(), 0.0f);
		saveTransform();

		renderInterfaceElements();

		glPopMatrix();
	}

	void Track::renderSpline(void)
	{
		if(mode == TEMPORAL)
		{
			assert(temporalSplineControl != NULL);
			temporalSplineControl->renderSpline();
		}
		else if(mode == FFT)
		{
			assert(fftSplineControl != NULL);
			fftSplineControl->renderSpline();
		}
	}

	void Track::setExitedCallback(ExitedCallback exited)
	{
		this->exited = exited;
	}

	void Track::setMode(TrackMode mode)
	{
		assert(menu != NULL);

		if(mode == FFT)
		{
			trackMenu_FFTSelected();
			menu->selectFFTMode();
		}
		else
		{
			trackMenu_TemporalSelected();
			menu->selectTemporalMode();
		}
	}

	/* PRIVATE MEMBER FUNCTIONS */
	void Track::initInterfaceElements(void)
	{
		assert(parameter != NULL);

		float buttonHeight = height / 2.0f;
		float buttonWidth = height / 3.0f;

		menu = new TrackMenu(Point2f(0.0f, 0.0f), height * TRACK_MENU_DIM, height, parameter->getName());
		menu->setFFTSelectedCallback(MakeDelegate(this, &Track::trackMenu_FFTSelected));
		menu->setTemporalSelectedCallback(MakeDelegate(this, &Track::trackMenu_TemporalSelected));
		menu->setPresetMenuToggledCallback(MakeDelegate(this, &Track::trackMenu_PresetMenuToggled));
		menu->setSavedCallback(MakeDelegate(this, &Track::trackMenu_Saved));
		menu->setResetCallback(MakeDelegate(this, &Track::trackMenu_Reset));
		registerInterfaceElement(menu);

		temporalSplineControl = new TemporalSplineControl(Point2f((height * TRACK_MENU_DIM) + BORDER_DIM, 0.0f), width, height, parameter->getDefaultValue());
		temporalSplineControl->setExitedCallback(MakeDelegate(this, &Track::spline_Exited));
		registerInterfaceElement(temporalSplineControl);

		fftSplineControl = new FFTSplineControl(Point2f((height * TRACK_MENU_DIM) + BORDER_DIM, 0.0f), width, height);
		fftSplineControl->setExitedCallback(MakeDelegate(this, &Track::spline_Exited));
		fftSplineControl->setEnabled(false);
		registerInterfaceElement(fftSplineControl);

		std::vector<SplinePreset *> splinePresets;
		presetMenu = new PresetMenu(Point2f((height * TRACK_MENU_DIM) + BORDER_DIM, 0.0f), height, splinePresets);
		presetMenu->setEnabled(false);
		presetMenu->setItemSelectedCallback(MakeDelegate(this, &Track::PresetMenu_ItemSelected));
		registerInterfaceElement(presetMenu);
	}

	void Track::PresetMenu_ItemSelected(SplinePreset *preset)
	{
		assert(preset != NULL);
		assert(menu != NULL);
		assert(fftSplineControl != NULL);
		assert(temporalSplineControl != NULL);
		assert(presetMenu != NULL);

		if(preset->getPresetType() == SplinePreset::FFT)
		{
			mode = FFT;
			menu->selectFFTMode();

			fftSplineControl->loadPreset(preset);

			temporalSplineControl->setEnabled(false);
			fftSplineControl->setEnabled(true);
		}
		else
		{
			mode = TEMPORAL;
			menu->selectTemporalMode();

			temporalSplineControl->loadPreset(preset);

			temporalSplineControl->setEnabled(true);
			fftSplineControl->setEnabled(false);
		}

		presetMenu->setEnabled(false);
		presetMenuDisplayed = false;
		menu->deSelectLoadButton();
	}

	void Track::spline_Exited(void)
	{
		assert(parameter != NULL);

		parameter->setValue(parameter->getDefaultValue());
		if(exited != NULL) 
		{
			exited(this);
		}
	}

	void Track::trackMenu_FFTSelected(void)
	{
		assert(temporalSplineControl != NULL);
		assert(fftSplineControl != NULL);
		assert(menu != NULL);
		assert(presetMenu != NULL);

		if(mode != FFT)
		{
			mode = FFT;
			temporalSplineControl->setEnabled(false);
			fftSplineControl->setEnabled(true);
			menu->selectFFTMode();
		}

		if(presetMenuDisplayed)
		{
			presetMenu->setEnabled(false);
			presetMenuDisplayed = false;
			menu->deSelectLoadButton();
		}
	}

	void Track::trackMenu_PresetMenuToggled(bool selected)
	{
		assert(presetMenu != NULL);
		assert(temporalSplineControl != NULL);
		assert(fftSplineControl != NULL);	
		
		if(selected)
		{
			WavesGlobalState *wgs = WavesGlobalState::getInstance();
			assert(wgs != NULL);

			std::map<unsigned int, SplinePreset *> &loadedPresets = wgs->getSplinePresets();
			std::map<unsigned int, SplinePreset *> &defaultPresets = wgs->getDefaultPresets();

			std::vector<SplinePreset *> menuItems;
			if(mode == TEMPORAL)
			{
				// defaults
				std::map<unsigned int, SplinePreset *>::iterator it;
				for(it = defaultPresets.begin(); it != defaultPresets.end(); it++)
				{
					SplinePreset *sp = (*it).second;
					assert(sp != NULL);

					if(sp->getPresetType() == SplinePreset::TEMPORAL)
					{
						menuItems.push_back(sp);
					}
				}

				// user defined presets
				for(unsigned int i = 0; i < temporalPresets.size(); i++)
				{
					unsigned int pId = temporalPresets[i];
					if(loadedPresets.count(pId) > 0)
					{
						SplinePreset *sp = loadedPresets[pId];			
						menuItems.push_back(sp);
					}
				}
			}
			else
			{
				// defaults
				std::map<unsigned int, SplinePreset *>::iterator it;
				for(it = defaultPresets.begin(); it != defaultPresets.end(); it++)
				{
					SplinePreset *sp = (*it).second;
					assert(sp != NULL);

					if(sp->getPresetType() == SplinePreset::FFT)
					{
						menuItems.push_back(sp);
					}
				}

				// user defined presets
				for(unsigned int i = 0; i < fftPresets.size(); i++)
				{
					unsigned int pId = fftPresets[i];
					if(loadedPresets.count(pId) > 0)
					{
						SplinePreset *sp = loadedPresets[pId];
						menuItems.push_back(sp);
					}
				}
			}
			
			presetMenu->setMenuItems(menuItems);
			presetMenu->setDirty(true);

			temporalSplineControl->setEnabled(false);
			fftSplineControl->setEnabled(false);
		}
		else if(!selected)
		{
			if(mode == TEMPORAL)
			{
				temporalSplineControl->setEnabled(true);
			}
			else
			{
				fftSplineControl->setEnabled(true);
			}
		}

		presetMenu->setEnabled(selected);
		presetMenuDisplayed = selected;
	}

	void Track::trackMenu_Reset(void)
	{
		assert(temporalSplineControl != NULL);
		assert(fftSplineControl != NULL);
		assert(menu != NULL);
		assert(presetMenu != NULL);

		if(mode == TEMPORAL)
		{
			temporalSplineControl->reset();
			temporalSplineControl->setEnabled(true);
		}
		else if(mode == FFT)
		{
			fftSplineControl->reset();
			fftSplineControl->setEnabled(true);
		}

		if(presetMenuDisplayed)
		{
			presetMenu->setEnabled(false);
			presetMenuDisplayed = false;
			menu->deSelectLoadButton();
		}
	}

	void Track::trackMenu_Saved(void)
	{
		assert(temporalSplineControl != NULL);
		assert(fftSplineControl != NULL);
		assert(menu != NULL);
		assert(presetMenu != NULL);

		unsigned int pId = 0;
		if(mode == TEMPORAL)
		{
			temporalSplineControl->savePreset();		
			pId = temporalSplineControl->getPresetId();
			addTemporalPreset(pId);
		}
		else if(mode == FFT)
		{
			fftSplineControl->savePreset();
			pId = fftSplineControl->getPresetId();
			addFFTPreset(pId);
		}

		if(presetMenuDisplayed)
		{
			presetMenu->setEnabled(false);
			presetMenuDisplayed = false;
			menu->deSelectLoadButton();

			if(mode == TEMPORAL)
			{
				temporalSplineControl->setEnabled(true);
			}
			else
			{
				fftSplineControl->setEnabled(true);
			}
		}
	}

	void Track::trackMenu_TemporalSelected(void)
	{
		assert(temporalSplineControl != NULL);
		assert(fftSplineControl != NULL);
		assert(menu != NULL);
		assert(presetMenu != NULL);

		if(mode != TEMPORAL)
		{
			mode = TEMPORAL;
			fftSplineControl->setEnabled(false);		
			temporalSplineControl->setEnabled(true);
			menu->selectTemporalMode();

			if(presetMenuDisplayed)
			{
				presetMenu->setEnabled(false);
				presetMenuDisplayed = false;
				menu->deSelectLoadButton();
			}
		}
	}

}
