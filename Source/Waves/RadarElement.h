/**
 * Class:	RadarElement
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */
#ifndef RadarElement_H
#define RadarElement_H

#include "Finger.h"
#include "InterfaceElement.h"

namespace Waves
{
	class RadarElement : 
		public InterfaceElement
	{
	public:
		RadarElement(const Point2f &position, float width, float height, float vanishingY);
		~RadarElement(void);

		bool contains(const FingerEventArgs &e) const;
		void fingerAdded(const FingerEventArgs &e);
		void fingerUpdated(const FingerEventArgs &e);
		void fingerRemoved(const FingerEventArgs &e);
		void render(void);
	protected: 
		static const unsigned int CIRCLE_VERTICES = 64;
		static const float LINE_OPACITY;
		static const float OPACITY;
		//static float VERTICAL_MOVE_TOLERANCE;

		Finger *controlFinger;
		std::map<int, Finger *> fingers;
		bool trackingFinger;
		float vanishingY;

		float getNonOffsetY(const Point2f &p) const;
		virtual float getOffsetY(float yVal, float zVal) const;
		float renderOpacity;

	};
}
#endif
