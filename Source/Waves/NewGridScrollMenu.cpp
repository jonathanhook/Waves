/**
 * Class:	NewGridScrollMenu
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */
#include "GridScrollMenuItem.h"
#include "NewGridScrollMenu.h"

namespace Waves
{
	/* CONSTRUCTORS */
	NewGridScrollMenu::NewGridScrollMenu(const Point2f &position, float width, float height, std::vector<Visual *> visuals) :
		GridScrollMenu(position, width, height)
	{
		this->visuals = visuals;

		initInterfaceElements();
	}

	NewGridScrollMenu::~NewGridScrollMenu(void)
	{
	}

	/* PUBLIC MEMBER FUNCTIONS */
	void NewGridScrollMenu::setVisualSelectedCallback(VisualSelectedCallback visualSelected)
	{
		this->visualSelected = visualSelected;
	}

	/* PRIVATE MEMBER FUNCTIONS */
	void NewGridScrollMenu::gridScrollMenuItem_VisualSelected(Visual *visual)
	{
		if(visualSelected != NULL)
		{
			visualSelected(visual);
		}
	}

	void NewGridScrollMenu::initInterfaceElements(void)
	{
		float miWidth = itemWidth;

		for(unsigned int i = 0; i < visuals.size(); i++)
		{
			float xPos = miWidth * (float)(i % GRID_DIM);
			float yPos = itemHeight * (float)(i / GRID_DIM);

			GridScrollMenuItem *mi = new GridScrollMenuItem(Point2f(xPos, yPos), miWidth, itemHeight, visuals[i]);
			mi->setVisualSelected(MakeDelegate(this, &NewGridScrollMenu::gridScrollMenuItem_VisualSelected));
			registerInterfaceElement(mi);
		}
	}
}
