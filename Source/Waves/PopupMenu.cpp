/**
 * Class:	PopupMenu
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */
#include <math.h>
#include "BPMControl.h"
#include "DragButton.h"
#include "ExitButton.h"
#include "InterfaceManager.h"
#include "LoadGridScrollMenu.h"
#include "NewGridScrollMenu.h"
#include "ExternalVisualScrollMenu.h"
#include "PopupMenuItem.h"
#include "PopupMenu.h"
#include "RadButton.h"
#include "RadButtonGroup.h"
#include "JDHUtility/Vector2f.h"
#include "VisualWavesControl.h"
#include "WavesGlobalState.h"

using namespace WavesVisuals;

namespace Waves
{
	/* STATIC VARS */
	float PopupMenu::BACKGROUND_OPACITY = 0.05f;
	float PopupMenu::EXIT_BUTTON_DIM = 0.25f;
	int PopupMenu::MAX_DIM = 3;

	/* CONSTRUCTORS */
	PopupMenu::PopupMenu(const Point2f &position, float width, float height) :
		InterfaceElement(position, width, height)
	{
		menuState = NEW;

		initInterfaceElements();
	}

	PopupMenu::~PopupMenu(void)
	{

	}

	/* PUBLIC MEMBER FUNCTIONS */
	void PopupMenu::fingerAdded(const FingerEventArgs &e)
	{

	}

	void PopupMenu::fingerUpdated(const FingerEventArgs &e)
	{

	}

	void PopupMenu::fingerRemoved(const FingerEventArgs &e)
	{

	}

	void PopupMenu::render(void)
	{
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();

		glTranslatef(position.getX(), position.getY(), 0.0f);
		saveTransform();

		glPushAttrib(GL_LINE_BIT | GL_CURRENT_BIT);

		glLineWidth(1.0f);
		if(dirty)
		{
			glNewList(displayList, GL_COMPILE);

			renderBackgroundMask();

			glColor4f(1.0f, 1.0f, 1.0f, LINE_OPACITY);
			glBegin(GL_LINE_STRIP);
				glVertex3f(width, BAR_DIM, 0.0f);
				glVertex3f(width, height, 0.0f);
				glVertex3f(0.0f, height, 0.0f);
				glVertex3f(0.0f, BAR_DIM, 0.0f);
			glEnd();

			glEndList();
		}
		glCallList(displayList);
	
		renderInterfaceElements();

		glPopAttrib();
		glPopMatrix();
	}

	void PopupMenu::setExitedCallback(ExitedCallback exited)
	{
		this->exited = exited;
	}

	void PopupMenu::setGrayed(VisualWavesControl *control, bool grayed)
	{
		assert(loadGrid != NULL);

		loadGrid->setGrayed(control, grayed);
	}

	void PopupMenu::setSavedControlSelectedCallback(SavedControlSelectedCallback savedControlSelected)
	{
		this->savedControlSelected = savedControlSelected;
	}

	void PopupMenu::setVisualSelectedCallback(VisualSelectedCallback visualSelected)
	{
		this->visualSelected = visualSelected;
	}

	/* PRIVATE MEMBER FUNCTIONS */
	void  PopupMenu::bpmTab_Clicked(void)
	{
		assert(newGrid != NULL);
		assert(loadGrid != NULL);
		assert(bpmControl != NULL);
		assert(exVisGrid != NULL);

		switch(menuState)
		{
			case NEW:
				newGrid->setEnabled(false);
				break;
			case LOAD:
				loadGrid->setEnabled(false);
				break;
			case EXVIS:
				exVisGrid->setEnabled(false);
				break;

		}

		bpmControl->setEnabled(true);
		menuState = BPM;
	}

	void PopupMenu::exitButton_Clicked(void)
	{
		if(exited != NULL) 
		{
			exited();
		}
	}
	
	void PopupMenu::loadGrid_SavedControlSelected(VisualWavesControl *control)
	{
		if(savedControlSelected != NULL)
		{
			savedControlSelected(control);
		}
	}

	void PopupMenu::loadTab_Clicked(void)
	{
		assert(newGrid != NULL);
		assert(bpmControl != NULL);
		assert(loadGrid != NULL);
		assert(exVisGrid != NULL);

		switch(menuState)
		{
			case NEW:
				newGrid->setEnabled(false);
				break;
			case BPM:
				bpmControl->setEnabled(false);
				break;
			case EXVIS:
				exVisGrid->setEnabled(false);
				break;
		}

		loadGrid->setEnabled(true);
		menuState = LOAD;
	}

	void PopupMenu::initInterfaceElements(void)
	{
		// init exit button
		float barDim = BAR_DIM * 3.0f;

		float dim = width * EXIT_BUTTON_DIM;
		ExitButton *exitButton = new ExitButton(Point2f(width - barDim, 0.0f), barDim, barDim);
		exitButton->setClickedCallback(MakeDelegate(this, &PopupMenu::exitButton_Clicked));
		registerInterfaceElement(exitButton);
	
		// status bar
		DragButton* topBar = new DragButton(Point2f(0.0f, 0.0f), width - barDim, barDim);
		topBar->setText("Menu");
		topBar->setDraggedCallback(MakeDelegate(this, &PopupMenu::topBar_Dragged));
		registerInterfaceElement(topBar);

		// tabs
		RadButtonGroup *tabGroup = new RadButtonGroup();

		float borderDim = (BORDER_DIM * 3.0f);
		float tabDim = barDim;
		float radX = borderDim;
		float radY = barDim + borderDim;

		std::string newStr = "New";
		RadButton *newTab = new RadButton(Point2f(radX, radY), newStr.size() * tabDim, tabDim, tabGroup, true);
		newTab->setClickedCallback(MakeDelegate(this, &PopupMenu::newTab_Clicked));
		newTab->setText(newStr);
		registerInterfaceElement(newTab);
		radX += newTab->getWidth();
		
		std::string loadStr = "Load";
		RadButton *loadTab = new RadButton(Point2f(radX, radY), loadStr.size() * tabDim, tabDim, tabGroup, false);
		loadTab->setClickedCallback(MakeDelegate(this, &PopupMenu::loadTab_Clicked));
		loadTab->setText(loadStr);
		registerInterfaceElement(loadTab);
		radX += loadTab->getWidth();
		
		std::string bpmStr = "BPM";
		RadButton *bpmTab = new RadButton(Point2f(radX, radY), bpmStr.size() * tabDim, tabDim, tabGroup, false);
		bpmTab->setClickedCallback(MakeDelegate(this, &PopupMenu::bpmTab_Clicked));
		bpmTab->setText(bpmStr);
		registerInterfaceElement(bpmTab);
		radX += bpmTab->getWidth();
		/*
		std::string visStr = "ExVis";
		RadButton *visTab = new RadButton(Point2f(radX, radY), 4.0f * tabDim, tabDim, tabGroup, false); //HACK:
		visTab->setClickedCallback(MakeDelegate(this, &PopupMenu::exVis_Clicked));
		visTab->setText(visStr);
		registerInterfaceElement(visTab);
		
*/
		WavesGlobalState *wavesGlobalState = WavesGlobalState::getInstance();
		assert(wavesGlobalState != NULL);

		const std::vector<Visual *> &factoryVisuals = wavesGlobalState->getFactoryVisuals();

		newGrid = new NewGridScrollMenu(Point2f(borderDim, radY + tabDim), width - (borderDim * 2.0f), height - barDim - tabDim - (borderDim * 2.0f), factoryVisuals);
		newGrid->setVisualSelectedCallback(MakeDelegate(this, &PopupMenu::newGrid_VisualSelected));
		registerInterfaceElement(newGrid);

		loadGrid = new LoadGridScrollMenu(Point2f(borderDim, radY + tabDim), width - (borderDim * 2.0f), height - barDim - tabDim - (borderDim * 2.0f));
		loadGrid->setEnabled(false);
		loadGrid->setSavedControlSelectedCallback(MakeDelegate(this, &PopupMenu::loadGrid_SavedControlSelected));
		registerInterfaceElement(loadGrid);

		bpmControl = new BPMControl(Point2f(borderDim, radY + tabDim), width - (borderDim * 2.0f), height - barDim - tabDim - (borderDim * 2.0f));
		bpmControl->setEnabled(false);
		registerInterfaceElement(bpmControl);
	
		//exVisGrid = new ExternalVisualScrollMenu(Point2f(borderDim, radY + tabDim), width - (borderDim * 2.0f), height - barDim - tabDim - (borderDim * 2.0f), factoryVisuals);
		//exVisGrid->setEnabled(false);
		//exVisGrid->setVisualSelectedCallback(MakeDelegate(this, &PopupMenu::exVis_VisualSelected));
		//registerInterfaceElement(exVisGrid);


	}

	void PopupMenu::newGrid_VisualSelected(Visual *visual)
	{
		if(visualSelected != NULL)
		{
			visualSelected(visual);
		}
	}

	void PopupMenu::newTab_Clicked(void)
	{
		assert(bpmControl != NULL);
		assert(loadGrid != NULL);
		assert(newGrid != NULL);
		assert(exVisGrid != NULL);

		switch(menuState)
		{
			case BPM:
				bpmControl->setEnabled(false);
				break;
			case LOAD:
				loadGrid->setEnabled(false);
				break;
			case EXVIS:
				exVisGrid->setEnabled(false);
				break;
		}

		newGrid->setEnabled(true);
		menuState = NEW;
	}

	void PopupMenu::topBar_Dragged(const Vector2f &v)
	{
		assert(modelview != NULL);

		position.translateX(v.getX() / modelview->getXScale());
		position.translateY(v.getY() / modelview->getYScale());
	}

	void PopupMenu::exVis_Clicked(void)
	{
		assert(bpmControl != NULL);
		assert(loadGrid != NULL);
		assert(newGrid != NULL);
		assert(exVisGrid != NULL);

		switch(menuState)
		{
			case NEW:
				newGrid->setEnabled(false);
				break;
			case BPM:
				bpmControl->setEnabled(false);
				break;
			case LOAD:
				loadGrid->setEnabled(false);
				break;
		}

		exVisGrid->setEnabled(true);
		menuState = EXVIS;
	}

	void PopupMenu::exVis_VisualSelected(Visual *visual)
	{
		if(visualSelected != NULL)
		{
			visualSelected(visual);
		}
	}
}