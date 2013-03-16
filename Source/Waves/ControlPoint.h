/**
 * Class:	ControlPoint
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */
#ifndef ControlPoint_H
#define ControlPoint_H

#include "FastDelegate.h"
#include "MultiTouchEvents/FingerEventArgs.h"
#include "InterfaceElement.h"

using namespace fastdelegate;
using namespace JDHUtility;

namespace Waves
{
	class ControlPoint :
		public InterfaceElement
	{
	public:
		typedef FastDelegate2<ControlPoint &, const FingerEventArgs &> FingerDownCallback;
		typedef FastDelegate2<ControlPoint &, const FingerEventArgs &> FingerDraggedCallback;
		typedef FastDelegate2<ControlPoint &, const FingerEventArgs &> FingerUpCallback;

		static bool compare(ControlPoint* lhs, ControlPoint* rhs);

		ControlPoint(const Point2f &position, float size);
		~ControlPoint(void);
 
		bool contains(const FingerEventArgs &e) const;
		void fingerAdded(const FingerEventArgs &e);
		void fingerUpdated(const FingerEventArgs &e);
		void fingerRemoved(const FingerEventArgs &e);
		void render(void);
		void setFingerDragged(FingerDraggedCallback fingerDragged);
		void setFingerDown(FingerDownCallback fingerDown);
		void setFingerUp(FingerUpCallback fingerUp);
		void setInside(bool inside);
	private:
		static const float BOUNDING_SPHERE;
		static const int CIRCLE_VERTICES;
		static const float CROSS_DIM;
		static const float OUTSIDE_OPACITY;
		static const float SELECTED_SIZE_FACTOR;

		FingerDraggedCallback fingerDragged;
		FingerDownCallback fingerDown;
		FingerUpCallback fingerUp;
		bool inside;
	};
}
#endif
