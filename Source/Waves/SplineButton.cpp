/**
 * Class:	SplineButton
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */
#include "JDHUtility/GlutInclude.h"
#include "JDHUtility/Interpolator.h"
#include "SplineButton.h"

using namespace JDHUtility;

namespace Waves
{
	/* CONSTRUCTORS */
	SplineButton::SplineButton(const Point2f &position, float width, float height, float *points, unsigned int pointCount) :
		Button(position, width, height)
	{
		this->points = points;
		this->pointCount = pointCount;

		splineDirty = true;
		SplineButtonDisplayList = glGenLists(1);
	}

	SplineButton::~SplineButton(void)
	{
		if(points != NULL) delete [] points;
		points = NULL;
	}

	/* PUBLIC MEMBER FUNCTIONS */
	void SplineButton::render(void)
	{
		Button::render();

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();

		glTranslatef(position.getX(), position.getY(), 0.0f);
		saveTransform();

		glLineWidth(getLineWidth(ICON_LINE_WIDTH));
		if(splineDirty)
		{
			glNewList(SplineButtonDisplayList, GL_COMPILE);

			float increment = width / (float)pointCount;

			glEnable(GL_LINE_SMOOTH);
			glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
			glBegin(GL_LINE_STRIP);
				for(unsigned int i = 0; i < pointCount; i++)
				{
					for(float f = 0; f < 1.0f; f+= 0.1f)
					{
						float x = (increment * (float)i) + (increment * f); 

						float y0 = points[(i - 1) % pointCount];
						float y1 = points[i];
						float y2 = points[(i + 1) % pointCount];
						float y3 = points[(i + 2) % pointCount];
						float y = Interpolator::hermiteInterpolate(y0, y1, y2, y3, f, Interpolator::DEFAULT_TENSION, 0.0f);	
						
						glVertex3f(x, y * height, 0.0f);

						if(i == (pointCount - 1) && f >= (1.0f - 0.1f))
						{
							glVertex3f(width, y * height, 0.0f);
						}
					}
				}
			glEnd();
			glDisable(GL_LINE_SMOOTH);

			glEndList();

			splineDirty = false;
		}
		glCallList(SplineButtonDisplayList);

		glPopMatrix();
	}
}
