/**
 * Class:	MultiTouchEventManager
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */
#include <assert.h>
#include "JDHUtility/Ndelete.h"
#include "EventSource.h"
#include "FastDelegate.h"
#include "FastDelegateBind.h"
#include "MultiTouchEvents/FingerEventArgs.h"
#include "MultiTouchEvents/MultiTouchEventHandler.h"
#include "MultiTouchEvents/MultiTouchEventManager.h"
#include "JDHUtility/Point2f.h"

using namespace fastdelegate;
using namespace JDHUtility;

namespace MultiTouchEvents
{
	/* CONSTRUCTORS */
	MultiTouchEventManager::MultiTouchEventManager(bool dispatchEvents)
	{
		this->dispatchEvents = dispatchEvents;

		peekEnabled = true;

#		ifdef _WIN32
		hMutex = CreateMutex(NULL, FALSE, NULL);  
#		endif
	}

	MultiTouchEventManager::~MultiTouchEventManager(void)
	{
		while(!eventQueue.empty())
		{
			FingerEventArgs *e = eventQueue.front();
			eventQueue.pop();
			NDELETE(e);
		}
#		ifdef _WIN32
		if (hMutex != NULL) { CloseHandle((HANDLE)hMutex); }
#		endif
	}

	/* PUBLIC MEMBER FUNCTIONS */
	void MultiTouchEventManager::dispatchAllEvents(void)
	{
#		ifdef _WIN32
		WaitForSingleObject((HANDLE)hMutex, INFINITE);	// TODO: Check return is WAIT_OBJECT_0
#		endif
		while(!eventQueue.empty())
		{
			FingerEventArgs *e = eventQueue.front();
			eventQueue.pop();
			dispatchEvent(e);
		}
#		ifdef _WIN32
		ReleaseMutex((HANDLE)hMutex);
#		endif
	}

	bool MultiTouchEventManager::isPeekEnabled(void)
	{
		return peekEnabled;
	}

	void MultiTouchEventManager::raiseEvent(unsigned int id, float x, float y, FingerEventArgs::EventType type)
	{
		// HACK : better than it was before
		if(type == FingerEventArgs::FINGER_REMOVED || (x > 0.001f && y > 0.001f))
		{
			Point2f p(x, y);
			FingerEventArgs *e = new FingerEventArgs(id, p, type);

			if(dispatchEvents) 
			{
				dispatchEvent(e);
			}
			else 
			{
#				ifdef _WIN32
				WaitForSingleObject((HANDLE)hMutex, INFINITE);	// TODO: Check return is WAIT_OBJECT_0
#				endif
				eventQueue.push(e);
#				ifdef _WIN32
				ReleaseMutex((HANDLE)hMutex);
#				endif
			}
		}
	}

	void MultiTouchEventManager::raiseKeyEvent(unsigned char key, bool pressed)
	{
		for(vector<MultiTouchEventHandler *>::iterator it = handlers.begin(); it != handlers.end(); it++)
		{
			MultiTouchEventHandler *handler = (*it);
			assert(handler != NULL);

			handler->processKeyEvent(key,  pressed);
		}
	}

	void MultiTouchEventManager::registerEventHandler(MultiTouchEventHandler *eventHandler)
	{
		handlers.push_back(eventHandler);
	}

	void MultiTouchEventManager::registerEventSource(EventSource *source)
	{
		assert(source);
		source->setEventCallback(MakeDelegate(this, &MultiTouchEventManager::raiseEvent));
	}

	void MultiTouchEventManager::release(unsigned int id)
	{
		if(captured.count(id) > 0)
		{
			MultiTouchEventHandler *cid = captured[id];
			assert(cid);

			cid->setMarkedForRelease(true);
		}
	}

	void MultiTouchEventManager::setPeekEnabled(bool peekEnabled)
	{
		this->peekEnabled = peekEnabled;
	}

	void MultiTouchEventManager::unregisterEventHandler(MultiTouchEventHandler *eventHandler)
	{
		for(vector<MultiTouchEventHandler *>::iterator it = handlers.begin(); it != handlers.end(); it++)
		{
			if(*it == eventHandler)
			{
				handlers.erase(it);
				break;
			}
		}
	}

	/* PRIVATE MEMBER FUNCTIONS */
	void MultiTouchEventManager::dispatchAddedEvent(FingerEventArgs *e)
	{
		unsigned int id = e->getId();
		
		for(vector<MultiTouchEventHandler *>::iterator it = handlers.begin(); it != handlers.end(); it++)
		{
			MultiTouchEventHandler *capturedHandler = (*it)->searchEventTree(*e);
			if(capturedHandler != NULL)
			{
				captured[id] = capturedHandler;
				captured[id]->capture(this);
				captured[id]->fingerAdded(*e);
				break;
			}
		}
	}

	void MultiTouchEventManager::dispatchEvent(FingerEventArgs *e)
	{
		assert(e != NULL);

		for(std::map<int, MultiTouchEventHandler *>::iterator it = captured.begin(); it != captured.end(); )
		{
			MultiTouchEventHandler *h = (*it).second;
			assert(h);

			if(h->isMarkedForRelease())
			{
				it = captured.erase(it);
				h->setMarkedForRelease(false);
			}
			else it++;
		}

		// let any peekers peek (WARNING: has been known to send events out of order)
		if(peekEnabled)
		{
			peek(*e);
		}

		// dispatch the event
		switch(e->getType())
		{
			case FingerEventArgs::EventType::FINGER_ADDED:
				dispatchAddedEvent(e);
				break;
			case FingerEventArgs::EventType::FINGER_REMOVED:
				dispatchRemovedEvent(e);
				break;
			case FingerEventArgs::EventType::FINGER_UPDATED:
				if(captured.count(e->getId()) > 0)
				{
					dispatchUpdatedEvent(e);
				}
				else
				{
					dispatchAddedEvent(e);
				}
				break;
		}

		NDELETE(e);
	}

	void MultiTouchEventManager::dispatchRemovedEvent(FingerEventArgs *e)
	{
		assert(e);

		unsigned int id = e->getId();
		
		if(captured.count(id) > 0)
		{
			MultiTouchEventHandler *cid = captured[id];
			assert(cid);

			cid->fingerRemoved(*e);
			cid->release();
			captured.erase(id);
		}
	}

	void MultiTouchEventManager::dispatchUpdatedEvent(FingerEventArgs *e)
	{
		assert(e);

		unsigned int id = e->getId();

		if(captured.count(id) > 0)
		{
			MultiTouchEventHandler *cid = captured[id];
			assert(cid);

			cid->fingerUpdated(*e);
		}
	}

	void MultiTouchEventManager::peek(const FingerEventArgs &e)
	{

	}
}