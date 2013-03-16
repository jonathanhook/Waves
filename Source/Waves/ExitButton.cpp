/**
 * Class:	ExitButton
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */
#include "ExitButton.h"
#include "JDHUtility/GlutInclude.h"

namespace Waves
{
	/* CONSTRUCTORS */
	ExitButton::ExitButton(const Point2f &position, float width, float height) :
		Button(position, width, height)
	{
		exitDirty = true;
		exitDisplayList = glGenLists(1);
	}

	ExitButton::~ExitButton(void)
	{

	}

	/* PUBLIC MEMBER FUNCTIONS */
	void ExitButton::render(void)
	{
		Button::render();
	
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();

		glTranslatef(position.getX(), position.getY(), 0.0f);
		saveTransform();

		glLineWidth(getLineWidth(ICON_LINE_WIDTH));
		if(exitDirty)
		{
			glNewList(exitDisplayList, GL_COMPILE);
	
			glPushAttrib(GL_LINE_BIT | GL_CURRENT_BIT);
			glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

			glEnable(GL_LINE_SMOOTH);
			glBegin(GL_LINE_STRIP);
				glVertex3f(width * 0.2f, height * 0.2f, 0.0f);
				glVertex3f(width * 0.8f, height * 0.8f, 0.0f);
			glEnd();

			glBegin(GL_LINE_STRIP);
				glVertex3f(width * 0.8f, height * 0.2f, 0.0f);
				glVertex3f(width * 0.2f, height * 0.8f, 0.0f);
			glEnd();
			glDisable(GL_LINE_SMOOTH);

			glPopAttrib();

			glEndList();

			exitDirty = false;
		}
		glCallList(exitDisplayList);

		glPopMatrix();	
	}
}
