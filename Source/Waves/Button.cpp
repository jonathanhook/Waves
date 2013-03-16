/**
 * Class:	Button
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */
#include <string>
#include "Button.h"
#include "Finger.h"
#include "JDHUtility/GLContext.h"
#include "JDHUtility/GLTexture.h"
#include "InterfaceElement.h"

using namespace JDHUtility;

namespace Waves
{
	/* STATIC VARS */
	const float			Button :: BACKGROUND_OPACITY	= 0.15f;
	const std::string	Button :: BUTTON_TEXTURE_KEY	= "ButtonTextureKey";
	const float			Button :: SELECTED_OPACITY		= 0.4f;
	const float			Button :: TEXT_BORDER			= 0.3f;

	/* CONSTRUCTORS */
	Button::Button(const Point2f &position, float width, float height) :
		InterfaceElement(position, width, height)
	{
		clicked			= NULL;
		hasTextureIcon	= false;
		pressed			= NULL;
		selected		= false;
		text			= "";
		textDirection	= HORIZONTAL;
	}

	Button::~Button(void)
	{

	}

	/* PUBLIC MEMBER FUNCTIONS */
	void Button::fingerAdded(const FingerEventArgs &e)
	{
		if(pressed != NULL) 
		{
			pressed();
		}

		selected = true;
		dirty = true;
	}

	void Button::fingerUpdated(const FingerEventArgs &e)
	{

	}

	void Button::fingerRemoved(const FingerEventArgs &e)
	{
		if(clicked != NULL && contains(e))
		{
			clicked();
		}

		if(lifted != NULL)
		{
			lifted();
		}

		selected = false;
		dirty = true;
	}

	void Button::render(void)
	{
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();

		glTranslatef(position.getX(), position.getY(), 0.0f);
		saveTransform();

		glLineWidth(1.0f);

		if(dirty)
		{
			glNewList(displayList, GL_COMPILE);
		
			glPushAttrib(GL_CURRENT_BIT | GL_LINE_BIT);

			renderBackgroundMask();

			// background
			if(selected) 
			{
				glColor4f(1.0f, 1.0f, 1.0f, SELECTED_OPACITY);
			}
			else 
			{
				glColor4f(1.0f, 1.0f, 1.0f, BACKGROUND_OPACITY);
			}

			glBegin(GL_QUADS);
				glVertex3f(0.0f, 0.0f, 0.0f);
				glVertex3f(width, 0.0f, 0.0f);
				glVertex3f(width, height, 0.0f);
				glVertex3f(0.0f, height, 0.0f);
			glEnd();

			glColor4f(1.0f, 1.0f, 1.0f, LINE_OPACITY);
			glBegin(GL_LINE_LOOP);
				glVertex3f(0.0f, 0.0f, 0.0f);
				glVertex3f(width, 0.0f, 0.0f);
				glVertex3f(width, height, 0.0f);
				glVertex3f(0.0f, height, 0.0f);
			glEnd();

			glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
			if(textDirection == VERTICAL)
			{
				glMatrixMode(GL_MODELVIEW);
				glPushMatrix();
				glTranslatef(width, 0.0f, 0.0f);
				glRotatef(90.0f, 0.0f, 0.0f, 1.0f);

				float textHeight = (width - BORDER_DIM);
				float textX = textHeight * text.length();
				
				std::string trimmedText = text;
				unsigned int overrun = 0;
				
				if(textX > height) 
				{
					overrun = (unsigned int)((textX - height) / width);
					trimmedText = text.substr(0, text.length() - overrun);
					textX = textHeight * trimmedText.size();
				}
				
				GLContext *currContext = GLContext::getCurrentContext();
				assert(currContext != NULL);	
				currContext->renderString(trimmedText, Vector2f(textX, width), TEXT_BORDER);

				glPopMatrix();
			}
			else
			{
				float textHeight = height;

				GLContext *currContext = GLContext::getCurrentContext();
				assert(currContext != NULL);	
				currContext->renderString(text, Vector2f(((textHeight * text.length()) * (1.0f - TEXT_BORDER)), textHeight), TEXT_BORDER);
			}


			glPopAttrib();

			glEndList();

			dirty = false;
		}

		glCallList(displayList);

		glPopMatrix();
	}

	void Button::setClickedCallback(ClickedCallback clicked)
	{
		this->clicked = clicked;
	}

	void Button::setLiftedCallback(LiftedCallback lifted)
	{
		this->lifted = lifted;
	}

	void Button::setPressedCallback(PressedCallback pressed)
	{
		this->pressed = pressed;
	}

	void Button::setText(const std::string &text)
	{
		this->text = text;
	}

	void Button::setTextDirection(TextDirection textDirection)
	{
		this->textDirection = textDirection;
	}
}