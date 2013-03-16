/**
 * Class:	FFTSplineControl
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */
#define _USE_MATH_DEFINES
#include <math.h>
#include "DragHandle.h"
#include "ExitButton.h"
#include "FastDelegate.h"
#include "FFTSplineControl.h"
#include "InterfaceManager.h"
#include "JDHUtility/Interpolator.h"
#include "FFT/LiveFFT.h"
#include "Playhead.h"
#include "JDHUtility/RollingAverage.h"
#include "SplinePreset.h"
#include "WavesGlobalState.h"

using namespace fastdelegate; 
using namespace JDHUtility;

namespace Waves
{
	/* CONSTRUCTORS */
	FFTSplineControl::FFTSplineControl(const Point2f &position, float width, float height) :
		SplineControl(position, width, height)
	{
		fftDirty = true;
		fftDisplayList = glGenLists(1);
		historyLength = InterfaceManager::BEATS * HISTORY_RESOLUTION;

		initInterfaceElements();
	}

	FFTSplineControl::~FFTSplineControl(void)
	{

	}

	/* PUBLIC MEMBER FUNCTIONS */
	float FFTSplineControl::getFFTResult(void)
	{
		WavesGlobalState *wgs = WavesGlobalState::getInstance();
		
		assert(wgs != NULL);
		const LiveFFT &fft = wgs->getFFT();
		unsigned int binCount;
		const RollingAverage<float> *fftVals = fft.getAveragedMusicalRangeOutput(binCount);

		// for each value of the fft find the spline position
		float xIncrement = width / (float)binCount;
		float totalFft = 0.0f;
		float totalSpline = 0.0f;

		for(unsigned int i = 0; i < binCount; i++)
		{
			float splineVal = getValue(xIncrement * (float)i);
			totalSpline += splineVal;

			const RollingAverage<float> *ra = &fftVals[i];

			assert(ra != NULL);
			float fftVal = ra->getAverage();
			if(fftVal > 1.0f) fftVal = 1.0f;
			totalFft += (fftVal * splineVal);
		}

		// guard against div by 0
		if(totalSpline == 0.0f)
		{
			return 0.0f;
		}
		else
		{
			float result = totalFft / totalSpline;
			return result;
		}
	}
	
	float FFTSplineControl::getValue(float beat)
	{
		float position = beat;
		int size = controlPoints.size();

		if(size <= 0) return 0.5f;
		else if(size <= 1) 
		{
			return getValueFromControlPoint(*controlPoints[0]);
		}
		else
		{
			for(int i = 0; i < size; i++)
			{
				ControlPoint *cp = controlPoints[i];
				assert(cp != NULL);

				if(cp->getPosition().getX() >= position)
				{
					int index0 = i - 2;
					int index1 = i - 1;
					int index3 = i + 1;
					
					if(index0 < 0) index0 = i;
					if(index1 < 0) index1 = i;
					if(index3 >= size) index3 = i;

					float y0 = getValueFromControlPoint(*controlPoints[index0]);
					float y1 = getValueFromControlPoint(*controlPoints[index1]);
					float y2 = getValueFromControlPoint(*controlPoints[i]);
					float y3 = getValueFromControlPoint(*controlPoints[index3]);

					ControlPoint *cpi = controlPoints[index1];
					assert(cpi != NULL);

					float x0 = constrainXVal(index1 == i ? 0.0f : cpi->getPosition().getX());
					float x1 = constrainXVal(cp->getPosition().getX());

					// in case of a wrap around		
					float mu = 1.0f; 
					
					// guard against / 0
					if(x0 != x1) 
					{
						mu = (position - x0) / (x1 - x0);
					}

					if(x0 > x1)
					{
						float interval = x1 + (width - x0);
						mu = (position + (width - x0)) / interval;
					}	

					float returnVal = Interpolator::hermiteInterpolate(y0, y1, y2, y3, mu, Interpolator::DEFAULT_TENSION, 0.0f);
					return constrainYVal(returnVal);
				}
			}
			
			unsigned int index0 = size - 2;
			unsigned int index1 = size - 1;
			if(index0 < 0) index0 = size - 1;

			// for last segment
			float y0 = getValueFromControlPoint(*controlPoints[index0]);
			float y1 = getValueFromControlPoint(*controlPoints[index1]);
			float y2 = getValueFromControlPoint(*controlPoints[index1]);
			float y3 = getValueFromControlPoint(*controlPoints[index1]);

			ControlPoint *cpi = controlPoints[index1];
			assert(cpi != NULL);

			float x0 = constrainXVal(cpi->getPosition().getX());
			float x1 = constrainXVal(width);

			float interval = x1 + (width - x0);
			float mu = (position - x0) / interval;
		
			float returnVal = Interpolator::hermiteInterpolate(y0, y1, y2, y3, mu, Interpolator::DEFAULT_TENSION, 0.0f);
			return constrainYVal(returnVal);
		}
	}

	void FFTSplineControl::loadPreset(SplinePreset *preset)
	{
		assert(preset != NULL);

		reset();

		float legendHeight = height * LEGEND_DIM;
		float legendlessHeight = height - (legendHeight * 2.0f);

		const std::vector<const Point2f> &points = preset->getPoints();
		for(unsigned int i = 0; i < points.size(); i++)
		{
			Point2f p(points[i].getX(), points[i].getY());
			p.scaleX(width);
			p.scaleY(legendlessHeight);
			p.translateY(legendHeight);
			registerControlPoint(p);
		}

		presetSaved = true;
		presetId = preset->getId();
	}

	void FFTSplineControl::render(void)
	{
		SplineControl::render();

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();

		glTranslatef(position.getX(), position.getY(), 0.0f);
		saveTransform();
		
		glPushAttrib(GL_CURRENT_BIT | GL_LINE_BIT);

		if(fftDirty)
		{
			glNewList(fftDisplayList, GL_COMPILE);

			glEndList();
			fftDirty = false;
		}
		glCallList(fftDisplayList);

		// no display list for FFT as it is always changing
		renderFFT();
		
		glPopAttrib();
		glPopMatrix();
	}

	void FFTSplineControl::renderSpline(void)
	{
		glPushAttrib(GL_LINE_BIT | GL_CURRENT_BIT);

		float value = getFFTResult();
		float legendlessHeight = height - ((height * LEGEND_DIM) * 2.0f);
		float vy = (height - (height * LEGEND_DIM)) - (value * legendlessHeight);

		glLineWidth(2.0f);
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glTranslatef(0.0f, vy, 0.0f);

		static int dl = -1;
		if(dl == -1)
		{
			dl = glGenLists(1);
			glNewList(dl, GL_COMPILE);

			glEnable(GL_LINE_SMOOTH);
			glBegin(GL_LINE_STRIP);
				glVertex3f(0.0f, 0.0f, 0.0f);
				glVertex3f(1.0f, 0.0f, 0.0f);
			glEnd();
			glDisable(GL_LINE_SMOOTH);

			glEndList();
		}
		glCallList(dl);

		glPopMatrix();
		glPopAttrib();
	}

	void FFTSplineControl::savePreset(void)
	{
		SplineControl::savePreset(SplinePreset::FFT);
	}

	/* PRIVATE MEMBER FUNCTIONS */
	void FFTSplineControl::expandButton_Dragged(const Vector2f &v)
	{
		assert(modelview != NULL);
		assert(expandButton != NULL);
		assert(exitButton != NULL);

		Point2f p(v.getX(), 0.0f);
		modelview->unScalePoint(p);

		float newWidth = width + p.getX();
		float beatWidth = InterfaceManager::getBeatLength();

		if(newWidth >= (beatWidth * MIN_BEAT_WIDTH) && newWidth <= (beatWidth * MAX_BEAT_WIDTH))
		{
			// expand/shrink the control
			float factor = newWidth / width;
			stretchControlPoints(factor);

			setWidth(newWidth);

			// move the handles
			expandButton->getPosition().setX(width + BORDER_DIM);
			exitButton->getPosition().setX(width + BORDER_DIM);

			dirty = true;
			fftDirty = true;
		}
	}

	void FFTSplineControl::initInterfaceElements(void)
	{
		exitButton = new ExitButton(Point2f(width + BORDER_DIM, 0.0f), BAR_DIM, BAR_DIM);
		exitButton->setClickedCallback(MakeDelegate(this, &SplineControl::exitButton_Clicked));
		registerInterfaceElement(exitButton);

		expandButton = new DragHandle(Point2f(width + BORDER_DIM, BAR_DIM), BAR_DIM, height - BAR_DIM, DragHandle::EXTEND);
		expandButton->setDraggedCallback(MakeDelegate(this, &FFTSplineControl::expandButton_Dragged));
		registerInterfaceElement(expandButton);
	}

	void FFTSplineControl::renderFFT(void)
	{
		WavesGlobalState *wgs = WavesGlobalState::getInstance();
		assert(wgs);

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glPushAttrib(GL_CURRENT_BIT | GL_LINE_BIT);

		const LiveFFT &fft = wgs->getFFT();
		unsigned int binCount;
		const RollingAverage<float> *fftVals = fft.getAveragedMusicalRangeOutput(binCount);

		glLineWidth(1.0f);
		glColor4f(1.0f, 1.0f, 1.0f, LINE_OPACITY);

		float xIncrement = width / (float)binCount;
		float yMax = height - ((height * LEGEND_DIM) * 2.0f);
		float yBase = height - (height * LEGEND_DIM);

		for(unsigned int i = 0; i < binCount; i++)
		{
			const RollingAverage<float> *ra = &fftVals[i];
			assert(ra != NULL);

			float val = ra->getAverage();
			if(val > 1.0f) val = 1.0f;
			float yVal = val * yMax;	

			float xMin = xIncrement * (float)i;
			float xMax = xIncrement * (float)(i + 1);

			glBegin(GL_LINE_LOOP);
				glVertex3f(xMin, yBase, 0.0f);
				glVertex3f(xMax, yBase, 0.0f);
				glVertex3f(xMax, yBase - yVal, 0.0f);	
				glVertex3f(xMin, yBase - yVal, 0.0f);
			glEnd();
		}

		glPopAttrib();
		glPopMatrix();
	}

	void FFTSplineControl::renderSplineControl(float splineControlWidth)
	{
		glPushAttrib(GL_LINE_BIT | GL_CURRENT_BIT);
		
		glColor4f(1.0f, 1.0f, 1.0, 1.0f);
		glLineWidth(getLineWidth(1.0f));
		glEnable(GL_LINE_SMOOTH);
		glBegin(GL_LINE_STRIP);
			float increment = InterfaceManager::getBeatLength() / (float)InterfaceManager::VALUES_PER_BEAT;
			for(float f = 0.0; f <= splineControlWidth; f+= (increment / (float)UPSAMPLE_FACTOR))
			{		
				float legendlessHeight = height - ((height * LEGEND_DIM) * 2.0f);
				float value = getValue(f);
				float vx = f;
				float vy = (value * legendlessHeight) + (height * LEGEND_DIM);
				glVertex3f(vx, height - vy, 0.0f);
			}
		glEnd();
		glDisable(GL_LINE_SMOOTH);
		
		glPopAttrib();
	}

	//void FFTSplineControl::saveHistoryVal(float beat, float val)
	//{
		//unsigned int index = (unsigned int)floor(beat * (float)historyLength);
		//history[index] = val;
	//}

	void FFTSplineControl::stretchControlPoints(float factor)
	{
		for(unsigned int i = 0; i < controlPoints.size(); i++)
		{
			ControlPoint *cp = controlPoints[i];
			assert(cp != NULL);

			float x = cp->getPosition().getX();
			x *= factor;
			cp->getPosition().setX(x);
		}
	}
}
