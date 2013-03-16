/**
 * Class:	SubtractButton
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */
#include "SubtractButton.h"
#include "JDHUtility/GlutInclude.h"

namespace Waves
{
	/* CONSTRUCTORS */
	SubtractButton::SubtractButton(const Point2f &position, float width, float height) :
		Button(position, width, height)
	{
		subtractButtonDisplayList = glGenLists(1);
	}

	SubtractButton::~SubtractButton(void)
	{
	}

	/* PUBLIC MEMBER FUNCTIONS */
	void SubtractButton::render(void)
	{
		Button::render();

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();

		glTranslatef(position.getX(), position.getY(), 0.0f);
		saveTransform();

		glPushAttrib(GL_CURRENT_BIT | GL_LINE_BIT);

		glLineWidth(ICON_LINE_WIDTH);
		if(subtractButtonDirty)
		{
			glNewList(subtractButtonDisplayList, GL_COMPILE);

			glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
			glBegin(GL_LINES);
				glVertex3f(width * 0.2f, height * 0.5f, 0.0f);
				glVertex3f(width * 0.8f, height * 0.5f, 0.0f);
			glEnd();

			glEndList();

			subtractButtonDirty = false;
		}
		glCallList(subtractButtonDisplayList);

		glPopAttrib();
		glPopMatrix();
	}
}