/**
 * Class:	CueButton
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */
#include "CueButton.h"
#include "JDHUtility/GlutInclude.h"

namespace Waves
{
	/* CONSTRUCTORS */
	CueButton::CueButton(const Point2f &position, float width, float height, bool toggled) :
		ToggleButton(position, width, height, toggled)
	{
		cueButtonDisplayList	= glGenLists(1);
		cueButtonDirty			= true;
	}

	CueButton::~CueButton(void)
	{
	}

	/* PUBLIC MEMBER FUNCTIONS */
	void CueButton::render(void)
	{
		Button::render();

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();

		glTranslatef(position.getX(), position.getY(), 0.0f);
		saveTransform();

		glPushAttrib(GL_CURRENT_BIT | GL_LINE_BIT);

		glLineWidth(getLineWidth(ICON_LINE_WIDTH));
		if(cueButtonDirty)
		{
			glNewList(cueButtonDisplayList, GL_COMPILE);
	
			// box
			glLineWidth(1.0f);
			glEnable(GL_LINE_SMOOTH);
			for(unsigned int i = 0; i < 2; i++)
			{
				float renderOpacity = (i == 0) ? 1.0f : 1.0f;
				glColor4f(1.0f, 1.0f, 1.0f, renderOpacity);
				
				GLenum renderMode = (i == 0) ? GL_QUADS : GL_LINE_LOOP;
				glBegin(renderMode);
					glVertex3f(width * 0.4f, height * 0.7f, 0.0f);
					glVertex3f(width * 0.8f, height * 0.7f, 0.0f);
					glVertex3f(width * 0.8f, height * 0.3f, 0.0f);
					glVertex3f(width * 0.4f, height * 0.3f, 0.0f);
				glEnd();

				renderMode = (i == 0) ? GL_TRIANGLES : GL_LINE_LOOP;
				glBegin(renderMode);
					glVertex3f(width * 0.2f, height * 0.7f, 0.0f);
					glVertex3f(width * 0.4f, height * 0.5f, 0.0f);
					glVertex3f(width * 0.2f, height * 0.3f, 0.0f);
				glEnd();
			}
			glDisable(GL_LINE_SMOOTH);

			glEndList();

			cueButtonDirty = false;
		}
		glCallList(cueButtonDisplayList);

		glPopAttrib();
		glPopMatrix();
	}
}
