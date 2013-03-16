/**
 * Class:	TextLabel
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */
#include "JDHUtility/GLContext.h"
#include "JDHUtility/GlutInclude.h"
#include "TextLabel.h"

using namespace JDHUtility;

namespace Waves
{
	/* CONSTRUCTORS */
	TextLabel::TextLabel(const Point2f &position, float width, float height, std::string text) :
		InterfaceElement(position, width, height)
	{
		this->text = text;
		centered = false;
	}

	TextLabel::~TextLabel(void)
	{
	}

	/* PUBLIC MEMBER FUNCTIONS */
	bool TextLabel::contains(const FingerEventArgs &e) const
	{
		return false;
	}

	void TextLabel::fingerAdded(const FingerEventArgs &e)
	{

	}

	void TextLabel::fingerUpdated(const FingerEventArgs &e)
	{

	}

	void TextLabel::fingerRemoved(const FingerEventArgs &e)
	{

	}

	void TextLabel::render(void)
	{
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();

		glTranslatef(position.getX(), position.getY(), 0.0f);
		saveTransform();

		glPushAttrib(GL_CURRENT_BIT | GL_LINE_BIT);

		if(dirty)
		{
			glNewList(displayList, GL_COMPILE);

			glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
			glLineWidth(1.0f);
			float tWidth = height * text.size();
			if(centered)
			{
				float xPos = (width / 2.0f) - (tWidth / 2.0f);
				glTranslatef(xPos, 0.0f, 0.0f);
			}

			GLContext *currContext = GLContext::getCurrentContext();
			assert(currContext != NULL);

			currContext->renderString(text, Vector2f(tWidth, height), 0.1f);

			glEndList();

			dirty = false;
		}
		glCallList(displayList);

		glPopAttrib();
		glPopMatrix();
	}

	void TextLabel::setCentered(bool centered)
	{
		this->centered = centered;
	}

	void TextLabel::setText(std::string text)
	{
		this->text = text;
		dirty = true;
	}
}
