/**
 * Class:	TuioEventSource
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */

// work-around for TUIO_CPP win32 definition error
#ifdef _WIN32
#define WIN32
#endif

#include <assert.h>
#include "JDHUtility/Ndelete.h"
#include "MultiTouchEvents/FingerEventArgs.h"
#include "JDHUtility/Point2f.h"
#include "TuioClient.h"
#include "TuioEventSource.h"

using namespace TUIO;

namespace MultiTouchEvents
{
	/* CONSTRUCTORS */
	TuioEventSource::TuioEventSource(const HWND &windowTarget, unsigned int port) :
		EventSource(windowTarget)
	{
		tuioClient = new TuioClient(port);
		tuioClient->addTuioListener(this);
		tuioClient->connect(false);
	}

	TuioEventSource::~TuioEventSource(void)
	{
		if(tuioClient != NULL)
		{
			tuioClient->disconnect();
			NDELETE(tuioClient);
		}
	}

	/* PUBLIC MEMBER FUNCTIONS */
	void TuioEventSource::addTuioCursor(TuioCursor *tcur)
	{
		assert(tcur != NULL);

		if(eventCallback != NULL) 
		{
			Point2f p(tcur->getX(), tcur->getY());

			if(windowTarget != NULL)
			{
				processPointForWindowTarget(p);
			}

			eventCallback(tcur->getSessionID(), p.getX(),p.getY(), FingerEventArgs::EventType::FINGER_ADDED);
		}
	}

	void TuioEventSource::removeTuioCursor(TuioCursor *tcur)
	{
		assert(tcur != NULL);

		if(eventCallback != NULL) 
		{
			Point2f p(tcur->getX(), tcur->getY());

			if(windowTarget != NULL)
			{
				processPointForWindowTarget(p);
			}

			eventCallback(tcur->getSessionID(), p.getX(), p.getY(), FingerEventArgs::EventType::FINGER_REMOVED);
		}
	}

	void TuioEventSource::updateTuioCursor(TuioCursor *tcur)
	{
		assert(tcur != NULL);
		
		if(eventCallback != NULL) 
		{
			Point2f p(tcur->getX(), tcur->getY());

			if(windowTarget != NULL)
			{
				processPointForWindowTarget(p);
			}

			eventCallback(tcur->getSessionID(), p.getX(), p.getY(), FingerEventArgs::EventType::FINGER_UPDATED);
		}
	}

	void TuioEventSource::addTuioObject(TuioObject *tobj) { /* NOT IMPLEMENTED */ }
	void TuioEventSource::refresh(TuioTime ftime) { /* NOT IMPLEMENTED */ }
	void TuioEventSource::removeTuioObject(TuioObject *tobj) { /* NOT IMPLEMENTED */ }
	void TuioEventSource::updateTuioObject(TuioObject *tobj) { /* NOT IMPLEMENTED */ }
}
