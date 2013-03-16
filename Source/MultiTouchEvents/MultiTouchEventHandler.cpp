/**
 * Class:	MultiTouchEventHandler
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */
#include <assert.h>
#include <vector>
#include "MultiTouchEvents/MultiTouchEventHandler.h"

namespace MultiTouchEvents
{
	/* CONSTRUCTORS */
	MultiTouchEventHandler::MultiTouchEventHandler(void)
	{
		captured = false;
		capturer = NULL;
		enabled = true;
		markedForRelease = false;
	}

	MultiTouchEventHandler::~MultiTouchEventHandler(void)
	{

	}

	/* PUBLIC MEMBER FUNCTIONS */
	void MultiTouchEventHandler::capture(MultiTouchEventManager *capturer)
	{
		this->capturer = capturer;
		captured = true;
	}

	MultiTouchEventManager *MultiTouchEventHandler::getCapturer(void) const
	{
		return capturer;
	}

	bool MultiTouchEventHandler::isCaptured(void) const
	{
		return captured;
	}

	bool MultiTouchEventHandler::isEnabled(void) const
	{
		return enabled;
	}

	bool MultiTouchEventHandler::isMarkedForRelease(void) const
	{
		return markedForRelease;
	}

	void MultiTouchEventHandler::keyPressed(unsigned char key)
	{

	}
	
	void MultiTouchEventHandler::keyReleased(unsigned char key)
	{

	}

	void MultiTouchEventHandler::processKeyEvent(unsigned char key, bool pressed)
	{
		if(pressed)
		{
			keyPressed(key);
		}
		else
		{
			keyReleased(key);
		}

		for(vector<MultiTouchEventHandler *>::iterator it = handlers.begin(); it != handlers.end(); it++)
		{
			MultiTouchEventHandler *child = (*it);
			assert(child != NULL);
			
			child->processKeyEvent(key, pressed);
		}
	}

	void MultiTouchEventHandler::registerEventHandler(MultiTouchEventHandler *eventHandler)
	{
		handlers.push_back(eventHandler);
	}

	void MultiTouchEventHandler::release(void)
	{
		capturer = NULL;
		captured = false;
	}

	MultiTouchEventHandler *MultiTouchEventHandler::searchEventTree(const FingerEventArgs &e)
	{
		if(enabled)
		{
			// search children
			for(vector<MultiTouchEventHandler *>::iterator it = handlers.begin(); it != handlers.end(); it++)
			{
				MultiTouchEventHandler *handled = (*it)->searchEventTree(e);

				if(handled != NULL) 
				{
					return handled;
				}
			}

			if(contains(e)) return this;		
		}
		
		return NULL;	
	}

	void MultiTouchEventHandler::setEnabled(bool enabled)
	{
		this->enabled = enabled;
	}

	void MultiTouchEventHandler::setMarkedForRelease(bool markedForRelease)
	{
		this->markedForRelease = markedForRelease;
	}

	void MultiTouchEventHandler::unregisterEventHandler(MultiTouchEventHandler *eventHandler)
	{
		for(std::vector<MultiTouchEventHandler *>::iterator it = handlers.begin(); it != handlers.end(); it++)
		{
			if(*it == eventHandler)
			{
				it = handlers.erase(it);
				break;
			}
		}
	}
}
