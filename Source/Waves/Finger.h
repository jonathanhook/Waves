/**
 * Class:	Finger
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */
#ifndef Finger_H
#define Finger_H

#include "JDHUtility/GlutInclude.h"
#include "InterfaceElement.h"
#include "JDHUtility/Point2f.h"

using namespace JDHUtility;

namespace Waves
{
	class Finger :
		public InterfaceElement
	{
	public:
		Finger(unsigned int fingerId, const Point2f &position, const Point2f &anchor);
		~Finger(void);

		bool contains(const FingerEventArgs &e) const;
		void fingerAdded(const FingerEventArgs &e);
		void fingerUpdated(const FingerEventArgs &e);
		void fingerRemoved(const FingerEventArgs &e);
		const Point2f &getAnchor(void) const;
		unsigned int getFingerId(void) const;
		const Point2f &getLast(void) const;
		unsigned int getTimestamp(void) const;
		void render(void);
		void setLast(const Point2f &position);
		void setTimestamp(unsigned int timestamp);
	private:
		static const float FINGER_RENDER_CIRCLE_OPACITY;
		static const int FINGER_RENDER_CIRCLE_RESOLUTION;
		static const float FINGER_RENDER_SIZE;

		unsigned int fingerId;
		Point2f anchor;
		Point2f last;
		unsigned int timestamp;
	};
}
#endif
