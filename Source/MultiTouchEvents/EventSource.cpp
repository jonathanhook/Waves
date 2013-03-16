/**
 * Class:	EventSource
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */
#include "EventSource.h"
#include "JDHUtility/WIN32Utils.h"

using namespace JDHUtility;

namespace MultiTouchEvents
{
	/* CONSTRUCTORS */
	EventSource::EventSource(HWND windowTarget)
	{
		this->windowTarget = windowTarget;
		this->eventCallback = NULL;
	}

	EventSource::~EventSource(void)
	{

	}

	/* PUBLIC MEMBER FUNCTIONS */
	void EventSource::setEventCallback(const EventCallback &eventCallback)
	{
		this->eventCallback = eventCallback;
	}

	void EventSource::setWindowTarget(const HWND &windowTarget)
	{
		this->windowTarget = windowTarget;
	}

	/* PROTECTED MEMBER FUNCTIONS */
	void EventSource::processPointForWindowTarget(Point2f &p) const
	{
		float wx, wy;
		WIN32Utils::screenPositionToWindowPosition(p.getX(), p.getY(), wx, wy, windowTarget, true);
		p.setX(wx);
		p.setY(wy);
	}
}