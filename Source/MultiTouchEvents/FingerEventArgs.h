/**
 * Class:	FingerEventArgs
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */
#ifndef FingerEventArgs_H
#define FingerEventArgs_H

#include "JDHUtility/Point2f.h"

using namespace JDHUtility;

namespace MultiTouchEvents
{
	class FingerEventArgs
	{
	public:
		typedef enum
		{
			FINGER_ADDED,
			FINGER_REMOVED,
			FINGER_UPDATED
		} EventType;

		FingerEventArgs(unsigned int id, const Point2f &position, EventType type);
		~FingerEventArgs(void);

		unsigned int getId(void) const;
		const Point2f &getPosition(void) const;
		EventType getType(void) const;
		float getX(void) const;
		float getY(void) const;

	protected:
		unsigned int id;
		Point2f position;
		EventType type;
	};
}
#endif
