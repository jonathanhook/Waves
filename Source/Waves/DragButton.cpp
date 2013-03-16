/**
 * Class:	DragButton
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */
#include <string>
#include <assert.h>
#include "DragButton.h"

namespace Waves
{
	/* CONSTRUCTORS */
	DragButton::DragButton(const Point2f &position, float width, float height) :
		Button(position, width, height)
	{
		last = NULL;
		trackingFinger = false;
	}

	DragButton::~DragButton(void)
	{
		if(last != NULL) delete last;
		last = NULL;
	}

	/* PUBLIC MEMBER FUNCTIONS */
	void DragButton::fingerAdded(const FingerEventArgs &e)
	{
		Button::fingerAdded(e);
		last = new Point2f(e.getPosition());
		trackingFinger = true;
	}

	void DragButton::fingerRemoved(const FingerEventArgs &e)
	{
		Button::fingerRemoved(e);
		last = NULL;
		trackingFinger = false;
	}

	void DragButton::fingerUpdated(const FingerEventArgs &e)
	{
		if(trackingFinger)
		{	
			assert(last != NULL);

			if(dragged != NULL) 
			{
				dragged(Vector2f(e.getX() - last->getX(), e.getY() - last->getY()));
			}

			last->setPosition(e.getPosition());
		}
	}

	void DragButton::setDraggedCallback(DraggedCallback dragged)
	{
		this->dragged = dragged;
	}
}
