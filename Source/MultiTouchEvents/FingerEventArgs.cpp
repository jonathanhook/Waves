/**
 * Class:	FingerEventArgs
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */
#include <stdio.h>
#include "MultiTouchEvents/FingerEventArgs.h"
#include "JDHUtility/Point2f.h"

using namespace JDHUtility;

namespace MultiTouchEvents
{
	/* CONSTRUCTORS */
	FingerEventArgs::FingerEventArgs(unsigned int id, const Point2f &position, EventType type)
	{
		this->id = id;
		this->position = position;
		this->type = type;
	}

	FingerEventArgs::~FingerEventArgs(void)
	{

	}

	/* PUBLIC MEMBER FUNCTIONS */
	unsigned int FingerEventArgs::getId(void) const
	{
		return id;
	}

	const Point2f &FingerEventArgs::getPosition(void) const
	{
		return position;
	}

	FingerEventArgs::EventType FingerEventArgs::getType(void) const
	{
		return type;
	}

	float FingerEventArgs::getX(void) const
	{
		return position.getX();
	}

	float FingerEventArgs::getY(void) const
	{
		return position.getY();
	}
}
