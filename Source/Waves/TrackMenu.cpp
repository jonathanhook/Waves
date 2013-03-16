/**
 * Class:	TrackMenu
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */
#include <vector>
#include "FFTButton.h"
#include "JDHUtility/GLContext.h"
#include "JDHUtility/GlutInclude.h"
#include "LoadButton.h"
#include "JDHUtility/Point2f.h"
#include "ResetButton.h"
#include "SaveButton.h"
#include "SplineButton.h"
#include "TextLabel.h"
#include "TrackMenu.h"

using namespace JDHUtility;

namespace Waves
{
	/* CONSTRUCTORS */
	TrackMenu::TrackMenu(const Point2f &position, float width, float height, std::string title) :
		InterfaceElement(position, width, height)
	{
		this->title = title;
		initInterfaceElements();
	}

	TrackMenu::~TrackMenu(void)
	{
	}

	/* PUBLIC MEMBER FUNCTIONS */
	void TrackMenu::deSelectLoadButton(void)
	{
		assert(loadButton != NULL);

		loadButton->setToggled(false);
	}

	void TrackMenu::fingerAdded(const FingerEventArgs &e)
	{

	}

	void TrackMenu::fingerUpdated(const FingerEventArgs &e)
	{

	}

	void TrackMenu::fingerRemoved(const FingerEventArgs &e)
	{

	}

	void TrackMenu::render(void)
	{
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();

		glTranslatef(position.getX(), position.getY(), 0.0f);
		saveTransform();

		glPushAttrib(GL_CURRENT_BIT | GL_LINE_BIT);

		if(dirty)
		{
			glNewList(displayList, GL_COMPILE);

			renderBackgroundMask();
			
			// border
			glLineWidth(1.0f);
			glColor4f(1.0f, 1.0f, 1.0f, LINE_OPACITY);
			glBegin(GL_LINE_LOOP);
				glVertex3f(0.0f, 0.0f, 0.0f);
				glVertex3f(width, 0.0f, 0.0f);
				glVertex3f(width, height, 0.0f);
				glVertex3f(0.0f, height, 0.0f);
			glEnd();

			glEndList();

			dirty = false;
		}
		glCallList(displayList);

		renderInterfaceElements();

		glPopAttrib();
		glPopMatrix();
	}

	void TrackMenu::selectFFTMode(void)
	{
		assert(fftModeButton != NULL);
		assert(temporalModeButton != NULL);

		fftModeButton->setEnabled(false);
		temporalModeButton->setEnabled(true);
	}
	
	void TrackMenu::selectTemporalMode(void)
	{
		assert(fftModeButton != NULL);
		assert(temporalModeButton != NULL);

		fftModeButton->setEnabled(true);
		temporalModeButton->setEnabled(false);
	}

	void TrackMenu::setFFTSelectedCallback(FFTSelectedCallback fftSelected)
	{
		this->fftSelected = fftSelected;
	}

	void TrackMenu::setPresetMenuToggledCallback(PresetMenuToggledCallback presetMenuToggled)
	{
		this->presetMenuToggled = presetMenuToggled;
	}

	void TrackMenu::setResetCallback(ResetCallback reset)
	{
		this->reset = reset;
	}

	void TrackMenu::setSavedCallback(SavedCallback saved)
	{
		this->saved = saved;
	}

	void TrackMenu::setTemporalSelectedCallback(TemporalSelectedCallback temporalSelected)
	{
		this->temporalSelected = temporalSelected;
	}

	/* PRIVATE MEMBER FUNCTIONS */
	void TrackMenu::fftModeButton_Clicked(void)
	{
		if(fftSelected != NULL) 
		{
			fftSelected();
		}
	}

	void TrackMenu::initInterfaceElements(void)
	{
		float borderDim = BORDER_DIM * 2.0f;
		float labelHeight = BAR_DIM / 2.5f;
		float buttonHeight = (height - ((borderDim * 3.0f) + labelHeight * 2.0f)) / 2.0f; 
		float buttonWidth = (width - (borderDim * 3.0f)) / 2.0f;

		//float buttonWidth = (width - (borderDim * 2.0f)) / 2.0f;

		/*
		float buttonWidth = (buttonRegionWidth - (borderDim * 4.0f)) / 2.0f;
		float buttonHeight = (height - (borderDim * 3.0f)) / 2.0f;
		*/

		

		// mode buttons
		TextLabel *nameLabel = new TextLabel(Point2f(borderDim, borderDim), width - (borderDim * 2.0f), labelHeight, title);
		registerInterfaceElement(nameLabel);

		unsigned int pointCount = 5;
		float *points  = new float[pointCount];
		points[0] = 0.6f;
		points[1] = 0.6f;
		points[2] = 0.2f;
		points[3] = 0.8f;
		points[4] = 0.6f;

		temporalModeButton = new SplineButton(Point2f(borderDim, borderDim + labelHeight + (BORDER_DIM * 2.5f)), buttonWidth, buttonHeight, points, pointCount);
		temporalModeButton->setClickedCallback(MakeDelegate(this, &TrackMenu::temporalModeButton_Clicked));
		temporalModeButton->setEnabled(false);
		registerInterfaceElement(temporalModeButton);
	
		fftModeButton = new FFTButton(Point2f(borderDim, borderDim + labelHeight + (BORDER_DIM * 2.5f)), buttonWidth, buttonHeight);
		fftModeButton->setClickedCallback(MakeDelegate(this, &TrackMenu::fftModeButton_Clicked));
		registerInterfaceElement(fftModeButton);

		ResetButton *resetButton = new ResetButton(Point2f(buttonWidth + (borderDim * 2.0f), borderDim + labelHeight + (BORDER_DIM * 2.5f)), buttonWidth, buttonHeight);
		resetButton->setClickedCallback(MakeDelegate(this, &TrackMenu::resetButton_Clicked));
		registerInterfaceElement(resetButton);

		loadButton = new LoadButton(Point2f(buttonWidth + (borderDim * 2.0f), buttonHeight + (borderDim * 1.0f) + (labelHeight * 2.0f) + (BORDER_DIM * 2.0f)), buttonWidth, buttonHeight);
		loadButton->setToggleChangedCallback(MakeDelegate(this, &TrackMenu::loadButton_ToggleChanged));
		registerInterfaceElement(loadButton);

		SaveButton *saveButton = new SaveButton(Point2f(borderDim, buttonHeight + (borderDim * 1.0f) + (labelHeight * 2.0f) + (BORDER_DIM * 2.0f)), buttonWidth, buttonHeight);
		saveButton->setClickedCallback(MakeDelegate(this, &TrackMenu::saveButton_Clicked));
		registerInterfaceElement(saveButton);


	}

	void TrackMenu::loadButton_ToggleChanged(bool selected)
	{
		if(presetMenuToggled != NULL) 
		{
			presetMenuToggled(selected);
		}
	}

	void TrackMenu::resetButton_Clicked(void)
	{
		if(reset != NULL)
		{
			reset();
		}
	}

	void TrackMenu::saveButton_Clicked(void)
	{
		if(saved != NULL)
		{
			saved();
		}
	}

	void TrackMenu::temporalModeButton_Clicked(void)
	{
		if(temporalSelected != NULL) 
		{
			temporalSelected();
		}
	}
}
