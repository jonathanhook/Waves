/**
 * Class:	FingerHeldCursor
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */
#ifndef FingerHeldCursor_H
#define FingerHeldCursor_H

#include "FastDelegate.h"
#include "InterfaceElement.h"

using namespace fastdelegate;

namespace Waves
{
	class FingerHeldCursor :
		public InterfaceElement
	{
	public:
		typedef FastDelegate1<FingerHeldCursor *> FingerHeldCallback;

		FingerHeldCursor(const Point2f &position, float size, unsigned int period);
		~FingerHeldCursor(void);

		bool contains(const FingerEventArgs &e) const;
		void fingerAdded(const FingerEventArgs &e);
		void fingerUpdated(const FingerEventArgs &e);
		void fingerRemoved(const FingerEventArgs &e);
		void render(void);
		void reset(void);
		void setFingerHeldCallback(FingerHeldCallback fingerHeld);
		void update(void);

	private:
		static const unsigned int CIRCLE_SEGS = 500;
		static const float INNER_RAD;

		unsigned int currentTime;
		FingerHeldCallback fingerHeld;
		bool fired;
		unsigned int period;
		unsigned int resetTime;
	};
}
#endif

