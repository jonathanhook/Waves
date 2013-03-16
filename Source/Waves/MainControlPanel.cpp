/**
 * Class:	MainControlPanel
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */
#include "CueButton.h"
#include "ExitButton.h"
#include "JDHUtility/GlutInclude.h"
#include "MainControlPanel.h"
#include "PreviewMonitor.h"
#include "ResetButton.h"
#include "SaveButton.h"

namespace Waves
{
	/* CONSTRUCTORS */
	MainControlPanel::MainControlPanel(const Point2f &position, float width, float height, Visual *visual, bool forOutput) :
		InterfaceElement(position, width, height)
	{
		this->visual = visual;
		this->forOutput = forOutput;

		initInterfaceElements();
	}

	MainControlPanel::~MainControlPanel(void)
	{
	}

	/* PUBLIC MEMBER FUNCTIONS */
	bool MainControlPanel::contains(const FingerEventArgs &e) const
	{
		return false;
	}

	void MainControlPanel::cueButton_ToggleChanged(bool selected)
	{
		if(cueChanged != NULL)
		{
			cueChanged(selected);
		}
	}

	void MainControlPanel::fingerAdded(const FingerEventArgs &e)
	{

	}

	void MainControlPanel::fingerUpdated(const FingerEventArgs &e)
	{

	}

	void MainControlPanel::fingerRemoved(const FingerEventArgs &e)
	{

	}

	void MainControlPanel::render(void)
	{
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glTranslatef(position.getX(), position.getY(), 0.0f);
		saveTransform();

		glPushAttrib(GL_LINE_BIT | GL_CURRENT_BIT);

		glLineWidth(0.75f);
		if(dirty)
		{
			glNewList(displayList, GL_COMPILE);

			renderBackgroundMask();

			glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
			glBegin(GL_LINE_LOOP);
				glVertex3f(0.0, 0.0f, 0.0f);
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

	void MainControlPanel::setClosedCallback(ClosedCallback closed)
	{
		this->closed = closed;
	}

	void MainControlPanel::setCueChangedCallback(CueChangedCallback cueChanged)
	{
		this->cueChanged = cueChanged;
	}

	void MainControlPanel::setSaveCallback(SaveCallback save)
	{
		this->save = save;
	}

	/* PRIVATE MEMBER FUNCTIONS */
	void  MainControlPanel::exitButton_Clicked(void)
	{
		if(closed != NULL)
		{
			closed();
		}
	}

	void MainControlPanel::initInterfaceElements(void)
	{
		float previewMonitorWidth = height * 1.33f;
		float borderDim = BORDER_DIM * 2.0f;
		
		PreviewMonitor *previewMonitor = new PreviewMonitor(Point2f(width - previewMonitorWidth - borderDim, borderDim), previewMonitorWidth, height - (borderDim * 2.0f), visual);
		registerInterfaceElement(previewMonitor);

		float buttonRegionWidth = width - previewMonitorWidth;
		float buttonWidth = (buttonRegionWidth - (borderDim * 4.0f)) / 2.0f;
		float buttonHeight = (height - (borderDim * 3.0f)) / 2.0f;

		CueButton *cueButton = new CueButton(Point2f(borderDim, borderDim), buttonWidth, buttonHeight);
		cueButton->setToggled(forOutput);
		cueButton->setToggleChangedCallback(MakeDelegate(this, &MainControlPanel::cueButton_ToggleChanged));
		registerInterfaceElement(cueButton);

		ResetButton *resetButton = new ResetButton(Point2f((borderDim * 2.0f) + buttonWidth, borderDim), buttonWidth, buttonHeight);
		registerInterfaceElement(resetButton);

		SaveButton *saveButton = new SaveButton(Point2f(borderDim, (borderDim * 2.0f) + buttonHeight), buttonWidth, buttonHeight);
		saveButton->setClickedCallback(MakeDelegate(this, &MainControlPanel::saveButton_Clicked));
		registerInterfaceElement(saveButton);

		ExitButton *exitButton = new ExitButton(Point2f(width - BAR_DIM, 0.0f), BAR_DIM, BAR_DIM);
		exitButton->setClickedCallback(MakeDelegate(this, &MainControlPanel::exitButton_Clicked));
		registerInterfaceElement(exitButton);
	}

	void MainControlPanel::saveButton_Clicked(void)
	{
		if(save != NULL)
		{
			save();
		}
	}
}

