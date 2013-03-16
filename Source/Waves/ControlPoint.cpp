/**
 * Class:	ControlPoint
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */
#define _USE_MATH_DEFINES
#include <math.h>
#include "ControlPoint.h"
#include "JDHUtility/GlutInclude.h"

namespace Waves
{
	/* PRIVATE CONSTANTS */
	const float ControlPoint::BOUNDING_SPHERE = 1.5f;
	const int ControlPoint::CIRCLE_VERTICES = 16;
	const float ControlPoint::CROSS_DIM = 0.33f;
	const float ControlPoint::OUTSIDE_OPACITY = 0.33f;
	const float ControlPoint::SELECTED_SIZE_FACTOR = 2.0f;

	/* PUBLIC STATIC FUNCTIONS */
	bool ControlPoint::compare(ControlPoint* lhs, ControlPoint* rhs)
	{
		assert(lhs != NULL);
		assert(rhs != NULL);

		return lhs->getPosition().getX() < rhs->getPosition().getX();
	}

	/* CONSTRUCTORS */
	ControlPoint::ControlPoint(const Point2f &position, float size) :
		InterfaceElement(position, size, size)
	{
		inside = true;
	}

	ControlPoint::~ControlPoint(void)
	{

	}

	/* PUBLIC MEMBER FUNCTIONS */
	bool ControlPoint::contains(const FingerEventArgs &e) const
	{
		Point2f p = e.getPosition();

		assert(modelview != NULL);
		modelview->unTranslatePoint(p);
		modelview->unScalePoint(p);

		float distance = sqrt((p.getX() * p.getX()) + (p.getY() * p.getY()));

		return distance <= (width * BOUNDING_SPHERE);
	}

	void ControlPoint::fingerAdded(const FingerEventArgs &e)
	{
		if(fingerDown != NULL) 
		{
			fingerDown(*this, e);
		}
	}

	void ControlPoint::fingerUpdated(const FingerEventArgs &e)
	{
		if(fingerDragged != NULL) 
		{
			fingerDragged(*this, e);
		}
	}

	void ControlPoint::fingerRemoved(const FingerEventArgs &e)
	{
		if(fingerUp != NULL) 
		{
			fingerUp(*this, e);
		}
	}

	void ControlPoint::render(void)
	{	
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();

		glTranslatef(position.getX(), position.getY(), 0.0f);
		saveTransform();

		float scale = width;
		glScalef(scale, scale, scale);

		glPushAttrib(GL_LINE_BIT | GL_CURRENT_BIT);

		if(dirty)
		{
			glNewList(displayList, GL_COMPILE);
		
			glLineWidth(1.0f);
			glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
			if(!inside) 
			{
				glColor4f(1.0f, 1.0f, 1.0f, OUTSIDE_OPACITY);
			}

			glBegin(GL_LINES);
				glVertex3f(CROSS_DIM, -CROSS_DIM, 0.0f);
				glVertex3f(-CROSS_DIM, CROSS_DIM, 0.0f);
				glVertex3f(CROSS_DIM, CROSS_DIM, 0.0f);
				glVertex3f(-CROSS_DIM, -CROSS_DIM, 0.0f);
			glEnd();

			glEndList();
		}

		glCallList(displayList);

		glPopAttrib();
		glPopMatrix();
	
	}

	void ControlPoint::setFingerDragged(FingerDraggedCallback fingerDragged)
	{
		this->fingerDragged = fingerDragged;
	}

	void ControlPoint::setFingerDown(FingerDownCallback fingerDown)
	{
		this->fingerDown = fingerDown;
	}

	void ControlPoint::setFingerUp(FingerUpCallback fingerUp)
	{
		this->fingerUp = fingerUp;
	}

	void ControlPoint::setInside(bool inside)
	{
		this->inside = inside;
	}
}