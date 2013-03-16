/**
 * Class:	LoadButton
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */
#define _USE_MATH_DEFINES
#include <math.h>
#include "LoadButton.h"
#include "JDHUtility/GlutInclude.h"

namespace Waves
{
	/* CONSTRUCTORS */
	LoadButton::LoadButton(const Point2f &position, float width, float height, bool toggled) :
		ToggleButton(position, width, height, toggled)
	{
		loadButtonDisplayList	= glGenLists(1);
		loadButtonDirty			= true;
	}

	LoadButton::~LoadButton(void)
	{
	}

	/* PUBLIC MEMBER FUNCTIONS */
	void LoadButton::render(void)
	{
		Button::render();

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();

		glTranslatef(position.getX(), position.getY(), 0.0f);
		saveTransform();

		glPushAttrib(GL_CURRENT_BIT | GL_LINE_BIT);

		glLineWidth(getLineWidth(ICON_LINE_WIDTH));
		if(loadButtonDirty)
		{
			glNewList(loadButtonDisplayList, GL_COMPILE);
	
			// box
			glLineWidth(1.0f);
			glEnable(GL_LINE_SMOOTH);
			for(unsigned int i = 0; i < 2; i++)
			{
				float renderOpacity = (i == 0) ? 1.0f : 1.0f;
				glColor4f(1.0f, 1.0f, 1.0f, renderOpacity);
				
				GLenum renderMode = (i == 0) ? GL_QUADS : GL_LINE_LOOP;
				glBegin(renderMode);
					glVertex3f(width * 0.3f, height * 0.8f, 0.0f);
					glVertex3f(width * 0.5f, height * 0.8f, 0.0f);
					glVertex3f(width * 0.5f, height * 0.4f, 0.0f);
					glVertex3f(width * 0.3f, height * 0.4f, 0.0f);
				glEnd();
				
				renderMode = (i == 0) ? GL_TRIANGLES : GL_LINE_LOOP;
				glBegin(renderMode);
					glVertex3f(width * 0.6f, height * 0.4f, 0.0f);
					glVertex3f(width * 0.4f, height * 0.2f, 0.0f);
					glVertex3f(width * 0.2f, height * 0.4f, 0.0f);
				glEnd();

				renderMode = (i == 0) ? GL_POLYGON : GL_LINE_LOOP;
				glBegin(renderMode);
					float radius = width * 0.075f;
					for(float f = 0.0f; f < 1.0f; f += 0.1f)
					{
						float theta = (2.0f * M_PI) * f;
						float x = (cos(theta) * radius) + (width * 0.7f);
						float y = (sin(theta) * radius) + (height * 0.8f);
						glVertex3f(x, y, 0.0f);
					}
				glEnd();
			}
			glDisable(GL_LINE_SMOOTH);

			glEndList();

			loadButtonDirty = false;
		}
		glCallList(loadButtonDisplayList);

		glPopAttrib();
		glPopMatrix();
	}
}
