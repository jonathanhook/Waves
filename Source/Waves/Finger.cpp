/**
 * Class:	Finger
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */
#define _USE_MATH_DEFINES
#include <math.h>
#include "Finger.h"

namespace Waves
{
	/* STATIC VARS */
	const float Finger::FINGER_RENDER_CIRCLE_OPACITY = 0.33f;
	const int Finger::FINGER_RENDER_CIRCLE_RESOLUTION = 64;
	const float Finger::FINGER_RENDER_SIZE = 0.025f;

	/* CONSTRUCTORS */
	Finger::Finger(unsigned int fingerId, const Point2f &position, const Point2f &anchor)
		: InterfaceElement(position, FINGER_RENDER_SIZE, FINGER_RENDER_SIZE)
	{
		this->fingerId = fingerId;
		this->anchor = anchor;
		this->last = anchor;

		timestamp = glutGet(GLUT_ELAPSED_TIME);
	}

	Finger::~Finger(void)
	{

	}

	/* PUBLIC MEMBER FUNCTIONS */
	bool Finger::contains(const FingerEventArgs &e) const
	{
		return false;
	}
	void Finger::fingerAdded(const FingerEventArgs &e)
	{

	}

	void Finger::fingerUpdated(const FingerEventArgs &e)
	{
		
	}

	void Finger::fingerRemoved(const FingerEventArgs &e)
	{

	}

	const Point2f &Finger::getAnchor(void) const
	{
		return anchor;
	}

	unsigned int Finger::getFingerId(void) const
	{
		return fingerId;
	}

	const Point2f &Finger::getLast(void) const
	{
		return last;
	}

	unsigned int Finger::getTimestamp(void) const
	{
		return timestamp;
	}

	void Finger::setLast(const Point2f &last)
	{
		this->last = last;
	}

	void Finger::setTimestamp(unsigned int timestamp)
	{
		this->timestamp = timestamp;
	}

	void Finger::render(void)
	{
		assert(modelview != NULL);

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glTranslatef(position.getX(), position.getY(), 0.0f);
		saveTransform();

		// HACK: reverse scale
		glScalef(1.0f / modelview->getXScale(), 1.0f / modelview->getYScale(), 1.0f);

		glPushAttrib(GL_LINE_BIT | GL_CURRENT_BIT);

		glLineWidth(0.75f);
		if(dirty)
		{
			glNewList(displayList, GL_COMPILE);

			// draw square
			glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
			glBegin(GL_LINE_LOOP);
				glVertex3f(-(width / 5.0f), -(width / 5.0f), 0.0f);
				glVertex3f(width / 5.0f, -(width / 5.0f), 0.0f);
				glVertex3f(width / 5.0f, width / 5.0f, 0.0f);
				glVertex3f(-(width / 5.0f), width / 5.0f, 0.0f);
			glEnd();

			// draw cross
			glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
			glBegin(GL_LINES);
				glVertex3f(0.0f, -(width / 2.0f), 0.0f);
				glVertex3f(0.0f, width / 2.0f, 0.0f);
				glVertex3f(-(width / 2.0f), 0.0f, 0.0f);
				glVertex3f(width / 2.0f, 0.0f, 0.0f);
			glEnd();

			glEndList();
			dirty = false;
		}
		glCallList(displayList);

		glPopAttrib();
		glPopMatrix();
	}
}
