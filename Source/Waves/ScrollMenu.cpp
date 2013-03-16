/**
 * Class:	ScrollMenu
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */
#include "ScrollMenu.h"

namespace Waves
{
	/* PROTECTED CONSTANTS */
	const float ScrollMenu::SCROLL_BAR_BORDER = 0.1f;

	/* CONSTRUCTORS */
	ScrollMenu::ScrollMenu(const Point2f &position, float width, float height, ScrollDirection scrollDirection, unsigned int itemCount) :
		InterfaceElement(position, width, height)
	{
		this->scrollDirection = scrollDirection;
		this->itemCount = itemCount;

		fingerDown = false;
		index = 0;
	}

	ScrollMenu::~ScrollMenu(void)
	{
	}

	/* PUBLIC MEMBER FUNCTIONS */
	void ScrollMenu::fingerAdded(const FingerEventArgs &e)
	{
		if(touched != NULL) touched();

		down = e.getPosition();
		dAxis = (scrollDirection == VERTICAL) ? e.getY() : e.getX();
		fingerDown = true;
		moved = false;
		selected = false;
	}
	
	void ScrollMenu::fingerUpdated(const FingerEventArgs &e)
	{
		assert(modelview != NULL);

		if(touched != NULL) 
		{
			touched();
		}

		float axisOffset = (scrollDirection == VERTICAL) ? dAxis - e.getY() : dAxis - e.getX();
		axisOffset /= modelview->getXScale();
		
		int difference = (int)(axisOffset / itemDim);
		
		if(difference != 0)
		{
			index += difference;

			if(index < 0) 
			{
				index = 0;
			}
			else if(index > (int)(itemCount - displayCount)) 
			{
				index = itemCount - displayCount;
			}

			dAxis = (scrollDirection == VERTICAL) ? e.getY() : e.getX();
			moved = true;
		}

		selected = false;
		dirty = true;
	}
	
	void ScrollMenu::fingerRemoved(const FingerEventArgs &e)
	{
		if(touched != NULL) touched();

		fingerDown = false;
		selected = false;
		dirty = true;
	}

	void ScrollMenu::setTouchedCallback(TouchedCallback touched)
	{
		this->touched = touched;
	}
	
	void ScrollMenu::update(void)
	{
		assert(modelview != NULL);

		if(!selected && fingerDown && !moved)
		{
			Point2f p = down;
			modelview->unTranslatePoint(p);
			modelview->unScalePoint(p);

			selected = true;
			selectedIndex = (scrollDirection == VERTICAL) ? (unsigned int)((p.getY() / height) * (float)displayCount) : (unsigned int)((p.getX() / width) * (float)displayCount);
			dirty = true;
		}
		else if(selected && moved)
		{
			selected = false;
			dirty = true;
		}
	}
}