/**
 * Class:	FFTSplineControl
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */
#ifndef FFTSplineControl_H
#define FFTSplineControl_H

#include "SplineControl.h"

namespace JDHUtility { class Vector2f; }
using namespace JDHUtility;

namespace Waves
{
	class DragHandle;
	class ExitButton;

	class FFTSplineControl : 
		public SplineControl
	{
	public:
		FFTSplineControl(const Point2f &position, float width, float height);
		~FFTSplineControl(void);
		
		float getFFTResult(void);
		float getValue(float beat);
		void loadPreset(SplinePreset *preset);
		void render(void);
		void renderSpline(void);
		void savePreset(void);

	private:
		static const unsigned int HISTORY_RESOLUTION = 16;
		static const unsigned int MIN_BEAT_WIDTH = 4;
		static const unsigned int MAX_BEAT_WIDTH = 32;

		ExitButton *exitButton;
		DragHandle *expandButton;		
		unsigned int fftDisplayList;
		bool fftDirty;
		//float *history;
		unsigned int historyLength;

		void expandButton_Dragged(const Vector2f &v);
		void initInterfaceElements(void);
		void renderFFT(void);
		void renderSplineControl(float splineControlWidth);
		//void saveHistoryVal(float beat, float val);
		void stretchControlPoints(float factor);
	};
}
#endif

