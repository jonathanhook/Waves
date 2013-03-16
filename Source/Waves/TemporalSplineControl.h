/**
 * Class:	TemporalSplineControl
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */
#ifndef TemporalSplineControl_H
#define TemporalSplineControl_H

#include <vector>
#include "JDHUtility/Point2f.h"
#include "SplineControl.h"
#include "JDHUtility/Vector2f.h"

namespace Waves
{
	class DragHandle;
	class ExitButton;
	class RadarHead;

	class TemporalSplineControl : 
		public SplineControl
	{
	public:
		TemporalSplineControl(const Point2f &position, float width, float height, float defaultValue);
		~TemporalSplineControl(void);

		float getValue(float beat);
		void loadPreset(SplinePreset *preset);
		void renderSpline(void);
		void savePreset(void);

	private:
		static const float PREVIEW_ALPHA_FACTOR;

		float defaultValue;
		DragHandle *expandButton;
		ExitButton *exitButton;
		RadarHead *radarHead;
		DragHandle *replicateButton;
		std::vector<float> snapValues;
		float snappedWidth;
		unsigned int temporalDisplayList;

		void expandButton_Dragged(const Vector2f &v);
		void expandButton_Lifted(void);
		void generateSnapValues(void);
		float getNewWidth(float dx);
		float getSnappedWidth(void);
		void initInterfaceElements(void);
		void renderSplineControl(float SplineControlWidth);
		void replicateButton_Dragged(const Vector2f &v);
		void replicateButton_Lifted(void);
		void replicateControlPoints(void);
		void snap(void);
		void trimControlPoints(void);
	};
}
#endif

