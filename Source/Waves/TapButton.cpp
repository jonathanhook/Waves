/**
 * Class:	TapButton
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */
#include "JDHUtility/GLContext.h"
#include "JDHUtility/GlutInclude.h"
#include "TapButton.h"

namespace Waves
{
	/* CONSTRUCTORS */
	TapButton::TapButton(const Point2f &position, float width, float height) :
		Button(position, width, height)
	{
		tapButtonDisplayList = glGenLists(1);
	}

	TapButton::~TapButton(void)
	{
	}

	/* PUBLIC MEMBER FUNCTIONS */
	void TapButton::render(void)
	{
		Button::render();

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();

		glTranslatef(position.getX(), position.getY(), 0.0f);
		saveTransform();

		glPushAttrib(GL_CURRENT_BIT | GL_LINE_BIT);

		glLineWidth(1.0f);
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		if(tapButtonDirty)
		{
			glNewList(tapButtonDisplayList, GL_COMPILE);
	
			float textHeight = height * 0.15f;
			float x = (width / 2.0f) - ((textHeight * tapText.size()) / 2.0f);
			float y = (height / 2.0f) - (textHeight / 2.0f);

			glTranslatef(x, y, 0.0f);

			GLContext *currContext = GLContext::getCurrentContext();
			assert(currContext != NULL);
			currContext->renderString(tapText, Vector2f(textHeight * tapText.size(), textHeight));

			glEndList();

			tapButtonDirty = false;
		}
		glCallList(tapButtonDisplayList);

		glPopAttrib();
		glPopMatrix();
	}

	void TapButton::reset(void)
	{
		tapText = "Tap ";
		tapButtonDirty = true;
	}

	void TapButton::setBpm(float bpm)
	{
		char buff[256];
		sprintf(buff, "%2.1f", bpm);

		tapText = buff;
		tapButtonDirty = true;
	}
}