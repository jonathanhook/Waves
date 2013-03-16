/**
 * Class:	DragButton
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */
#ifndef DragButton_H
#define DragButton_H

#include "Button.h"
#include "FastDelegate.h"
#include "MultiTouchEvents/FingerEventArgs.h"
#include "JDHUtility/Vector2f.h"

using namespace fastdelegate;
using namespace MultiTouchEvents;



namespace Waves
{
	class DragButton :
		public Button
	{
	public:
		typedef FastDelegate1<const Vector2f &> DraggedCallback;

		DragButton(const Point2f &position, float width, float height);
		~DragButton(void);

		void fingerAdded(const FingerEventArgs &e);
		void fingerRemoved(const FingerEventArgs &e);
		void fingerUpdated(const FingerEventArgs &e);
		void setDraggedCallback(DraggedCallback dragged);

	protected:
		DraggedCallback dragged;
		Point2f *last;
		bool trackingFinger;
	};
}
#endif
