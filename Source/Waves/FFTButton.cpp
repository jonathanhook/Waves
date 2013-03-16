/**
 * Class:	FFTButton
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */
#include "FFTButton.h"
#include "JDHUtility/GlutInclude.h"

namespace Waves
{
	/* CONSTRUCTORS */
	FFTButton::FFTButton(const Point2f &position, float width, float height) :
		Button(position, width, height)
	{
		fftButtonDirty = true;
		fftButtonDisplayList = glGenLists(1);
	}

	FFTButton::~FFTButton(void)
	{
	}

	/* PUBLIC MEMBER FUNCTIONS */
	void FFTButton::render(void)
	{
		Button::render();

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();

		glTranslatef(position.getX(), position.getY(), 0.0f);
		saveTransform();

		glPushAttrib(GL_CURRENT_BIT | GL_LINE_BIT);

		glLineWidth(1.0f);
		if(fftButtonDirty)
		{
			glNewList(fftButtonDisplayList, GL_COMPILE);

			glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
			glBegin(GL_LINE_LOOP);
				// bar 1
				glVertex3f(width * 0.15f, height * 0.2f, 0.0f);
				glVertex3f(width * 0.15f, height * 0.8f, 0.0f);
				glVertex3f(width * 0.25f, height * 0.8f, 0.0f);
				glVertex3f(width * 0.25f, height * 0.2f, 0.0f);
			glEnd();
			
			glBegin(GL_LINE_LOOP);	
				// bar 2
				glVertex3f(width * 0.3f, height * 0.5f, 0.0f);
				glVertex3f(width * 0.3f, height * 0.8f, 0.0f);
				glVertex3f(width * 0.4f, height * 0.8f, 0.0f);
				glVertex3f(width * 0.4f, height * 0.5f, 0.0f);
			glEnd();
				
			glBegin(GL_LINE_LOOP);
				// bar 3
				glVertex3f(width * 0.45f, height * 0.55f, 0.0f);
				glVertex3f(width * 0.45f, height * 0.8f, 0.0f);
				glVertex3f(width * 0.55f, height * 0.8f, 0.0f);
				glVertex3f(width * 0.55f, height * 0.55f, 0.0f);
			glEnd();
				
			glBegin(GL_LINE_LOOP);
				// bar 5
				glVertex3f(width * 0.6f, height * 0.33f, 0.0f);
				glVertex3f(width * 0.6f, height * 0.8f, 0.0f);
				glVertex3f(width * 0.7f, height * 0.8f, 0.0f);
				glVertex3f(width * 0.7f, height * 0.33f, 0.0f);
			glEnd();
				
			glBegin(GL_LINE_LOOP);
				// bar 5
				glVertex3f(width * 0.75f, height * 0.65f, 0.0f);
				glVertex3f(width * 0.75f, height * 0.8f, 0.0f);
				glVertex3f(width * 0.85f, height * 0.8f, 0.0f);
				glVertex3f(width * 0.85f, height * 0.65f, 0.0f);
			glEnd();

			glEndList();

			fftButtonDirty = false;
		}
		glCallList(fftButtonDisplayList);

		glPopAttrib();
		glPopMatrix();
	}
}
