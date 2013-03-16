/**
 * Class:	RadarSegment
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */
#ifndef RadarSegment_H
#define RadarSegment_H

#include "FastDelegate.h"
#include "RadarElement.h"

using namespace fastdelegate;

namespace Waves
{
	class WavesControl;

	class RadarSegment :
		public RadarElement
	{
	public:
		typedef FastDelegate1<RadarElement *> DetachedCallback;
		typedef FastDelegate1<RadarElement *> ShiftDownCallback;
		typedef FastDelegate1<RadarElement *> ShiftUpCallback;

		RadarSegment(const Point2f &position, float width, float height, float vanishingY, WavesControl *control = NULL);
		~RadarSegment(void);

		bool contains(const FingerEventArgs &e) const;
		WavesControl *getControl(void);
		void render(void);
	private:
		static float TEXTURE_OPACITY;

		WavesControl *control;
		DetachedCallback detached;
		ShiftDownCallback shiftDown;
		ShiftUpCallback shiftUp;

		void control_HeightChanged(void);
		void detach(void);
		virtual float getOffsetY(float yVal, float zVal) const;
	};
}
#endif
