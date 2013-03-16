/**
 * Class:	TemporalSplineControl
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */
#include "DragHandle.h"
#include "ExitButton.h"
#include "JDHUtility/Interpolator.h"
#include "InterfaceManager.h"
#include "RadarHead.h"
#include "SplinePreset.h"
#include "TemporalSplineControl.h"

using namespace JDHUtility;

namespace Waves
{	
	/* OPERATORS */
	// [jdh] WARNING: this operator does not work in the case a < -b
	static inline int mod(int a, int b) { return (a >= 0) ? (a == 0 ? 0 : a % b) : b + a; }

	/* PRIVATE CONSTANTS */
	const float TemporalSplineControl::PREVIEW_ALPHA_FACTOR = 0.25f; 

	/* CONSTRUCTORS */
	TemporalSplineControl::TemporalSplineControl(const Point2f &position, float width, float height, float defaultValue) :
		SplineControl(position, width, height)
	{
		this->defaultValue = defaultValue;

		snappedWidth = width;
		temporalDisplayList = glGenLists(1);

		generateSnapValues();
		initInterfaceElements();
	}

	TemporalSplineControl::~TemporalSplineControl(void)
	{
	}

	/* PUBLIC MEMBER FUNCTIONS */
	float TemporalSplineControl::getValue(float beat)
	{
		// if width 0 (in the case when a visual is used outside of the 
		// waves interface
		if(width <= 0.0f) return defaultValue;

		float position = fmodf(beat, width);
		int size = controlPoints.size();

		if(size <= 0) return defaultValue;
		else if(size <= 1) 
		{
			return getValueFromControlPoint(*controlPoints[0]);
		}
		else
		{
			for(int i = 0; i < size; i++)
			{
				ControlPoint *cpi = controlPoints[i];
				assert(cpi != NULL);

				if(cpi->getPosition().getX() >= position)
				{
					float y0 = getValueFromControlPoint(*controlPoints[mod(i - 2, size)]);
					float y1 = getValueFromControlPoint(*controlPoints[mod(i - 1, size)]);
					float y2 = getValueFromControlPoint(*controlPoints[mod(i, size)]);
					float y3 = getValueFromControlPoint(*controlPoints[mod(i + 1, size)]);

					ControlPoint *cpx0 = controlPoints[mod(i - 1, size)];
					assert(cpx0 != NULL);
					float x0 = constrainXVal(cpx0->getPosition().getX());

					ControlPoint *cpx1 = controlPoints[mod(i, size)];
					assert(cpx1 != NULL);
					float x1 = constrainXVal(cpx1->getPosition().getX());

					float xDist = (x1 - x0);
					float mu = (position - x0) / xDist;

					// in case of a wrap around	
					if(x0 > x1 || xDist == 0.0f)
					{
						float interval = x1 + (width - x0);
						if(interval == 0.0f) interval = width;

						mu = (position + (width - x0)) / interval;
					}

					float returnVal = Interpolator::hermiteInterpolate(y0, y1, y2, y3, mu, Interpolator::DEFAULT_TENSION, 0.0f);
					return constrainYVal(returnVal);
				}
			}
			
			// for last segment
			float y0 = getValueFromControlPoint(*controlPoints[mod(size - 2, size)]);
			float y1 = getValueFromControlPoint(*controlPoints[mod(size - 1, size)]);
			float y2 = getValueFromControlPoint(*controlPoints[0]);
			float y3 = getValueFromControlPoint(*controlPoints[mod(1, size)]);

			ControlPoint *cpx0 = controlPoints[mod(size - 1, size)];
			assert(cpx0 != NULL);
			float x0 = constrainXVal(cpx0->getPosition().getX());

			ControlPoint *cpx1 = controlPoints[0];
			assert(cpx1 != NULL);
			float x1 = constrainXVal(cpx1->getPosition().getX());

			float interval = x1 + (width - x0);
			if(interval == 0.0f) interval = width;

			float mu = (position - x0) / interval;

			float returnVal = Interpolator::hermiteInterpolate(y0, y1, y2, y3, mu, Interpolator::DEFAULT_TENSION, 0.0f);
			return constrainYVal(returnVal);
		}
	}

	void TemporalSplineControl::loadPreset(SplinePreset *preset)
	{
		assert(preset != NULL);

		reset();

		// set width
		setWidth(preset->getWidth());
		snap();
		
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

	void TemporalSplineControl::renderSpline(void)
	{
		glLineWidth(2.0f);
		if(fullLengthDirty)
		{
			glNewList(fullLengthDisplayList, GL_COMPILE);
			renderSplineControl(1.0f);
			glEndList();

			fullLengthDirty = false;
		}
		glCallList(fullLengthDisplayList);
	}

	void TemporalSplineControl::savePreset(void)
	{
		SplineControl::savePreset(SplinePreset::TEMPORAL);
	}

	/* PRIVATE MEMBER FUNCTIONS */
	void TemporalSplineControl::expandButton_Dragged(const Vector2f &v)
	{
		assert(expandButton != NULL);
		assert(replicateButton != NULL);
		assert(exitButton != NULL);

		float newWidth = getNewWidth(v.getX());

		// expand/shrink the control
		setWidth(newWidth);

		// move the handles
		expandButton->getPosition().setX(width + BORDER_DIM);
		replicateButton->getPosition().setX(width + BORDER_DIM);
		exitButton->getPosition().setX(width + BORDER_DIM);

		trimControlPoints();
		snappedWidth = getSnappedWidth();
		dirty = true;
		splineDirty = true;
	}

	void TemporalSplineControl::expandButton_Lifted(void)
	{
		snap();
		splineDirty = true;
	}

	void TemporalSplineControl::generateSnapValues(void)
	{
		float beatLength = InterfaceManager::getBeatLength();

		for(unsigned int i = 1; i < InterfaceManager::BEATS; i++)
		{
			if(InterfaceManager::BEATS % i == 0)
			{
				snapValues.push_back((float)i * InterfaceManager::getBeatLength());
			}
		}
	}

	float TemporalSplineControl::getNewWidth(float dx)
	{
		assert(modelview != NULL);

		Point2f p(dx, 0.0f);
		modelview->unScalePoint(p);

		// expand
		float newWidth = width += p.getX();
		if(newWidth < InterfaceManager::getBeatLength()) 
		{
			newWidth = InterfaceManager::getBeatLength();
		}
		else if(newWidth > (InterfaceManager::getBeatLength() * InterfaceManager::BEATS))
		{
			newWidth = InterfaceManager::getBeatLength() * InterfaceManager::BEATS;
		}

		return newWidth;
	}

	float TemporalSplineControl::getSnappedWidth(void)
	{
		for(unsigned int i = 0; i < snapValues.size(); i++)
		{
			if((width >= snapValues[i] && i == snapValues.size() - 1) ||
				(width >= snapValues[i] && width < snapValues[i + 1]))
			{	
				return snapValues[i];
			}
		}

		return width;
	}

	void TemporalSplineControl::initInterfaceElements(void)
	{
		float handleHeight = (height - BAR_DIM) / 2.0f;

		expandButton = new DragHandle(Point2f(width + BORDER_DIM, BAR_DIM), BAR_DIM, handleHeight, DragHandle::EXTEND);
		expandButton->setDraggedCallback(MakeDelegate(this, &TemporalSplineControl::expandButton_Dragged));
		expandButton->setLiftedCallback(MakeDelegate(this, &TemporalSplineControl::expandButton_Lifted));
		registerInterfaceElement(expandButton);

		replicateButton = new DragHandle(Point2f(width + BORDER_DIM, handleHeight + BAR_DIM), BAR_DIM, handleHeight, DragHandle::REPLICATE);
		replicateButton->setDraggedCallback(MakeDelegate(this, &TemporalSplineControl::replicateButton_Dragged));
		replicateButton->setLiftedCallback(MakeDelegate(this, &TemporalSplineControl::replicateButton_Lifted));
		registerInterfaceElement(replicateButton);

		radarHead = new RadarHead(Point2f(0.0f, 0.0f), width, height);
		registerInterfaceElement(radarHead);

		exitButton = new ExitButton(Point2f(width + BORDER_DIM, 0.0f), BAR_DIM, BAR_DIM);
		exitButton->setClickedCallback(MakeDelegate(this, &SplineControl::exitButton_Clicked));
		registerInterfaceElement(exitButton);
	}

	void TemporalSplineControl::renderSplineControl(float splineControlWidth)
	{
		glEnable(GL_LINE_SMOOTH);
		glBegin(GL_LINE_STRIP);
			float increment = InterfaceManager::getBeatLength() / (float)InterfaceManager::VALUES_PER_BEAT;
			for(float f = 0.0; f <= splineControlWidth; f+= (increment / (float)UPSAMPLE_FACTOR))
			{	
				// set color
				glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
				if(snappedWidth != 0.0f && (splineControlWidth == width && width != snappedWidth) && (f > snappedWidth))
				{
					glColor4f(1.0f, 1.0f, 1.0f, 1.0f * PREVIEW_ALPHA_FACTOR);
				}
				
				float legendlessHeight = height - ((height * LEGEND_DIM) * 2.0f);
				float value = getValue(f);
				float vx = f;
				float vy = (value * legendlessHeight) + (height * LEGEND_DIM);
				glVertex3f(vx, height - vy, 0.0f);
			}
		glEnd();
		glDisable(GL_LINE_SMOOTH);
	}

	void TemporalSplineControl::replicateButton_Dragged(const Vector2f &v)
	{
		assert(expandButton != NULL);
		assert(replicateButton != NULL);
		assert(exitButton != NULL);

		float newWidth = getNewWidth(v.getX());

		// expand/shrink the control
		setWidth(newWidth);

		// move the handles
		expandButton->getPosition().setX(width + BORDER_DIM);
		replicateButton->getPosition().setX(width + BORDER_DIM);
		exitButton->getPosition().setX(width + BORDER_DIM);

		if(v.getX() > 0) 
		{
			replicateControlPoints();
		}

		trimControlPoints();
		snappedWidth = getSnappedWidth();
		dirty = true;
		splineDirty = true;
	}
	
	void TemporalSplineControl::replicateButton_Lifted(void)
	{
		snap();
		splineDirty = true;
	}

	void TemporalSplineControl::replicateControlPoints(void)
	{
		if(controlPoints.size() > 0)
		{
			ControlPoint *last = controlPoints[controlPoints.size() - 1];
			assert(last != NULL);

			float lastX = last->getPosition().getX();

			std::vector<Point2f> toAdd;
			for(std::vector<ControlPoint*>::iterator it = controlPoints.begin(); it != controlPoints.end(); it++)
			{	
				ControlPoint *cp = *it;
				assert(cp != NULL);

				float nx = cp->getPosition().getX() + snappedWidth;

				if(nx < width && nx > lastX)
				{
					toAdd.push_back(Point2f(nx, cp->getPosition().getY()));
				}
			}

			for(std::vector<Point2f>::iterator it = toAdd.begin(); it != toAdd.end(); it++)
			{
				registerControlPoint(*it);
			}
			
		}
	}

	void TemporalSplineControl::snap(void)
	{
		assert(expandButton != NULL);
		assert(replicateButton != NULL);
		assert(exitButton != NULL);
		assert(radarHead != NULL);

		snappedWidth = getSnappedWidth();
		setWidth(snappedWidth);

		trimControlPoints();
		dirty = true;

		// move the handles
		expandButton->getPosition().setX(width + BORDER_DIM);
		replicateButton->getPosition().setX(width + BORDER_DIM);
		exitButton->getPosition().setX(width + BORDER_DIM);
		
		radarHead->setWidth(width);
	}


	void TemporalSplineControl::trimControlPoints(void)
	{
		for(vector<ControlPoint *>::iterator it = controlPoints.begin(); it != controlPoints.end(); )
		{
			ControlPoint *cp = (*it);
			assert(cp != NULL);

			if(cp->getPosition().getX() > width)
			{
				it = controlPoints.erase(it);
				deleteInterfaceElement(cp);
			}
			else it++;
		}
	}
}