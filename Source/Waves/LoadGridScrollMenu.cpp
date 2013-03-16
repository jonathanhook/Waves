/**
 * Class:	LoadGridScrollMenu
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */
#include "Button.h"
#include "GridScrollMenuItem.h"
#include "LoadGridScrollMenu.h"
#include "LoadGridScrollMenuItem.h"
#include "VisualWavesControl.h"
#include "WavesGlobalState.h"

namespace Waves
{
	/* CONSTRUCTORS */
	LoadGridScrollMenu::LoadGridScrollMenu(const Point2f &position, float width, float height) :
		GridScrollMenu(position, width, height)
	{
		currentSize = 0;
		initInterfaceElements();
	}

	LoadGridScrollMenu::~LoadGridScrollMenu(void)
	{
	}

	/* PUBLIC MEMBER FUNCTIONS */
	void LoadGridScrollMenu::setGrayed(VisualWavesControl *control, bool grayed)
	{
		std::map<int, InterfaceElement *>::iterator it;
		for(it = interfaceElements.begin(); it != interfaceElements.end(); it++)
		{
			LoadGridScrollMenuItem *lgsmi = dynamic_cast<LoadGridScrollMenuItem *>((*it).second);
			if(lgsmi != NULL)
			{
				assert(lgsmi != NULL);
				assert(control != NULL);

				const VisualWavesControl &v = lgsmi->getControl();
				if(v.getId() == control->getId())
				{
					lgsmi->setGrayed(grayed);
				}
			}
		}
	}

	void LoadGridScrollMenu::setSavedControlSelectedCallback(SavedControlSelectedCallback savedControlSelected)
	{
		this->savedControlSelected = savedControlSelected;
	}

	void LoadGridScrollMenu::update(void)
	{
		WavesGlobalState *wgs = WavesGlobalState::getInstance();
		assert(wgs != NULL);

		std::map<unsigned int, VisualWavesControl *> savedControls = wgs->getSavedControls();
		if(currentSize != savedControls.size())
		{
			initInterfaceElements();
		}

		updateInterfaceElements();
	}

	/* PROTECTED MEMBER FUNCTIONS */
	void LoadGridScrollMenu::initInterfaceElements(void)
	{
		/// clear the menu
		std::map<int, InterfaceElement *>::iterator it;
		for(it = interfaceElements.begin(); it != interfaceElements.end(); it++)
		{
			deleteInterfaceElement((*it).second);
		}
		
		WavesGlobalState *wgs = WavesGlobalState::getInstance();
		assert(wgs != NULL);

		std::map<unsigned int, VisualWavesControl *> savedControls = wgs->getSavedControls();
		currentSize = savedControls.size();

		float miWidth = itemWidth;
		
		std::map<unsigned int, VisualWavesControl *>::iterator it1;
		unsigned int i = 0;
		for(it1 = savedControls.begin(); it1 != savedControls.end(); it1++)
		{
			VisualWavesControl *visualWavesControl = (*it1).second;
			assert(visualWavesControl != NULL);

			float xPos = miWidth * (float)(i % GRID_DIM);
			float yPos = itemHeight * (float)(i / GRID_DIM);	
			
			LoadGridScrollMenuItem *mi = new LoadGridScrollMenuItem(Point2f(xPos, yPos), miWidth, itemHeight, visualWavesControl);
			assert(mi != NULL);

			mi->setSavedControlSelectedCallback(MakeDelegate(this, &LoadGridScrollMenu::loadGridScrollMenuItem_savedControlSelected));
			mi->setGrayed(visualWavesControl->isEnabled());
			registerInterfaceElement(mi);

			i++;
		}
		
	}

	/* PRIVATE MEMBER FUNCTIONS */
	void LoadGridScrollMenu::loadGridScrollMenuItem_savedControlSelected(VisualWavesControl *control)
	{
		if(savedControlSelected != NULL)
		{
			savedControlSelected(control);
		}
	}
}
