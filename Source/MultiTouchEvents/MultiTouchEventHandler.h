/**
 * Class:	MultiTouchEventHandler
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */
#ifndef MultiTouchEventHandler_H
#define MultiTouchEventHandler_H

#include <vector>
#include "MultiTouchEvents/MultiTouchEventManager.h"

using namespace std;

namespace MultiTouchEvents
{
	class FingerEventArgs;

	class MultiTouchEventHandler
	{
	public:
		MultiTouchEventHandler	(void);
		~MultiTouchEventHandler	(void);

		void					capture(MultiTouchEventManager *capturer);
		virtual bool			contains(const FingerEventArgs &e) const = 0;
		virtual void			fingerAdded(const FingerEventArgs &e) = 0;
		virtual void			fingerUpdated(const FingerEventArgs &e) = 0;
		virtual void			fingerRemoved(const FingerEventArgs &e) = 0;
		MultiTouchEventManager	*getCapturer(void) const;
		bool					isCaptured(void) const;
		bool					isEnabled(void) const;
		bool					isMarkedForRelease(void) const;
		virtual void			keyReleased(unsigned char key);
		virtual void			keyPressed(unsigned char key);
		void					processKeyEvent(unsigned char key, bool pressed);
		void					registerEventHandler(MultiTouchEventHandler *eventHandler);
		void					release(void);
		MultiTouchEventHandler	*searchEventTree(const FingerEventArgs &e);
		void					setEnabled(bool enabled);
		void					setMarkedForRelease(bool markedForRelease);
		void					unregisterEventHandler(MultiTouchEventHandler *eventHandler);

	private:
		MultiTouchEventManager				*capturer;
		vector<MultiTouchEventHandler *>	handlers;
		bool								captured;
		bool								enabled;
		bool								markedForRelease;
	};
}
#endif
