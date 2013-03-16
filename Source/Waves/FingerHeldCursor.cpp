/**
 * Class:	FingerHeldCursor
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */
#define _USE_MATH_DEFINES
#include <math.h>
#include "FingerHeldCursor.h"
#include "JDHUtility/GlutInclude.h"

namespace Waves
{
	/* PRIVATE CONSTANTS */
	const float FingerHeldCursor::INNER_RAD = 0.75f;

	/* CONSTRUCTORS */
	FingerHeldCursor::FingerHeldCursor(const Point2f &position, float size, unsigned int period) :
		InterfaceElement(position, size, size)
	{
		this->period = period;

		reset();
	}

	FingerHeldCursor::~FingerHeldCursor(void)
	{
	}

	/* PUBLIC MEMBER FUNCTIONS */
	bool FingerHeldCursor::contains(const FingerEventArgs &e) const
	{
		return false;
	}

	void FingerHeldCursor::fingerAdded(const FingerEventArgs &e)
	{

	}

	void FingerHeldCursor::fingerUpdated(const FingerEventArgs &e)
	{

	}

	void FingerHeldCursor::fingerRemoved(const FingerEventArgs &e)
	{

	}

	void FingerHeldCursor::render(void)
	{
		assert(modelview != NULL);

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();

		glTranslatef(position.getX(), position.getY(), 0.0f);
		saveTransform();

		glLineWidth(1.0f);
		if(dirty)
		{
			glNewList(displayList, GL_COMPILE);

			float scale = modelview->getXScale();

			float radius = (width / 4.0f) / scale;
			float iRadius = radius * INNER_RAD;

			unsigned int elapsedTime = currentTime - resetTime;
			float progress = (float)elapsedTime / (float)period;
			float increment = 1.0f / (float)CIRCLE_SEGS;

			glColor4f(1.0f, 1.0f, 1.0f, 0.9f);
			glBegin(GL_TRIANGLE_STRIP);
				for(float f = 0; f < progress; f += increment)
				{
					float theta = (2.0f * M_PI) * f;
					float x = cos(theta) * radius;
					float y = sin(theta) * radius;

					float ix = cos(theta) * iRadius;
					float iy = sin(theta) * iRadius;

					glVertex3f(x, y, 0.0f);
					glVertex3f(ix, iy, 0.0f);
				}
			glEnd();

			glEnable(GL_LINE_SMOOTH);
			glBegin(GL_LINE_STRIP);
				for(float f = 0; f < progress; f += increment)
				{
					float theta = (2.0f * M_PI) * f;
					float x = cos(theta) * radius;
					float y = sin(theta) * radius;

					glVertex3f(x, y, 0.0f);
				}
			glEnd();

			glBegin(GL_LINE_STRIP);
				for(float f = 0; f < progress; f += increment)
				{
					float theta = (2.0f * M_PI) * f;
					float ix = cos(theta) * iRadius;
					float iy = sin(theta) * iRadius;

					glVertex3f(ix, iy, 0.0f);
				}
			glEnd();
			glDisable(GL_LINE_SMOOTH);


			glEndList();
			dirty = false;
		}
		glCallList(displayList);

		glPopMatrix();
	}

	void FingerHeldCursor::reset(void)
	{
		resetTime = glutGet(GLUT_ELAPSED_TIME);
		currentTime = resetTime;
		fired = false;
	}
	
	void FingerHeldCursor::setFingerHeldCallback(FingerHeldCallback fingerHeld)
	{
		this->fingerHeld = fingerHeld;
	}

	void FingerHeldCursor::update(void)
	{
		dirty = true;
		currentTime = glutGet(GLUT_ELAPSED_TIME);

		unsigned int elapsedTime = currentTime - resetTime;
		if(elapsedTime > period && fingerHeld != NULL && !fired)
		{
			fired = true;
			fingerHeld(this);
		}		
	}
}
