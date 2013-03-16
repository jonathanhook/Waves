/**
 * Class:	AddButton
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */
#include "AddButton.h"
#include "JDHUtility/GlutInclude.h"

namespace Waves
{
	/* CONSTRUCTORS */
	AddButton::AddButton(const Point2f &position, float width, float height) :
		Button(position, width, height)
	{
		addButtonDisplayList = glGenLists(1);
	}

	AddButton::~AddButton(void)
	{
	}

	/* PUBLIC MEMBER FUNCTIONS */
	void AddButton::render(void)
	{
		Button::render();

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();

		glTranslatef(position.getX(), position.getY(), 0.0f);
		saveTransform();

		glPushAttrib(GL_CURRENT_BIT | GL_LINE_BIT);

		glLineWidth(ICON_LINE_WIDTH);
		if(addButtonDirty)
		{
			glNewList(addButtonDisplayList, GL_COMPILE);

			glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
			glEnable(GL_LINE_SMOOTH);
			glBegin(GL_LINES);
				glVertex3f(width * 0.5f, height * 0.8f, 0.0f);
				glVertex3f(width * 0.5f, height * 0.2f, 0.0f);
				glVertex3f(width * 0.2f, height * 0.5f, 0.0f);
				glVertex3f(width * 0.8f, height * 0.5f, 0.0f);
			glEnd();
			glDisable(GL_LINE_SMOOTH);

			glEndList();

			addButtonDirty = false;
		}
		glCallList(addButtonDisplayList);

		glPopAttrib();
		glPopMatrix();
	}
}
