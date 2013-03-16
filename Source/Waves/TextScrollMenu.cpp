/**
 * Class:	TextScrollMenu
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */
#include "JDHUtility/GLContext.h"
#include "JDHUtility/GlutInclude.h"
#include "TextScrollMenu.h"

namespace Waves
{
	/* PRIVATE CONSTANTS */
	const float TextScrollMenu::CHAR_SCALE = 0.5f;

	/* CONSTRUCTORS */
	TextScrollMenu::TextScrollMenu(const Point2f &position, float preferredHeight, std::vector<std::string> menuItems) :
		ScrollMenu(position, 0.0f, preferredHeight, ScrollMenu::VERTICAL, menuItems.size())
	{
		this->menuItems = menuItems;

		displayCount = menuItems.size() > DISPLAY_COUNT ? DISPLAY_COUNT : menuItems.size();
		itemSelected = NULL;

		itemDim = preferredHeight / (float)DISPLAY_COUNT;
		if(menuItems.size() < DISPLAY_COUNT)
		{
			height = itemDim * menuItems.size();
		}

		for(unsigned int i = 0; i < menuItems.size(); i++)
		{
			float thisLength = menuItems[i].size() * (itemDim * CHAR_SCALE);
			if(thisLength > width) width = thisLength;
		}

		width += itemDim * CHAR_SCALE;
	}

	TextScrollMenu::~TextScrollMenu(void) 
	{

	}

	/* PUBLIC MEMBER FUNCTIONS */
	void TextScrollMenu::fingerRemoved(const FingerEventArgs &e)
	{
		// if not moved then click
		if(selected && fingerDown && !moved)
		{
			if(itemSelected != NULL) itemSelected(menuItems[selectedIndex + index]);
		}
		
		ScrollMenu::fingerRemoved(e);
	}

	void TextScrollMenu::render(void)
	{
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();

		glTranslatef(position.getX(), position.getY(), 0.0f);
		saveTransform();

		glPushAttrib(GL_CURRENT_BIT | GL_LINE_BIT);

		glLineWidth(1.0f);
		if(dirty)
		{
			glNewList(displayList, GL_COMPILE);
			
			renderBackgroundMask();

			glColor4f(1.0f, 1.0f, 1.0f, LINE_OPACITY);
			glBegin(GL_LINE_STRIP);
				glVertex3f(0.0f, height, 0.0f);
				glVertex3f(0.0f, 0.0f, 0.0f);
				glVertex3f(width, 0.0f, 0.0f);
				glVertex3f(width, height, 0.0f);
			glEnd();

			renderScrollBar();

			for(unsigned int i = index; i < (displayCount + index); i++)
			{
				renderMenuItem(menuItems[i], (selected && (i - index == selectedIndex)));
				glTranslatef(0.0f, itemDim, 0.0f);
			}	

			glDisable(GL_TEXTURE_2D);
			glEndList();

			dirty = false;
		}
		glCallList(displayList);

		glPopAttrib();
		glPopMatrix();
	}

	void TextScrollMenu::setItemSelected(ItemSelectedCallback itemSelected)
	{
		this->itemSelected = itemSelected;
	}

	/* PRIVATE MEMBER FUNCTIONS */
	void TextScrollMenu::renderMenuItem(std::string text, bool selected)
	{
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();

		glColor4f(1.0f, 1.0f, 1.0f, LINE_OPACITY);
		glBegin(GL_LINE_STRIP);
			glVertex3f(width, itemDim, 0.0f);
			glVertex3f(0.0f, itemDim, 0.0f);
		glEnd();

		if(selected)
		{
			glColor4f(1.0f, 1.0f, 1.0f, 0.25f);
			glBegin(GL_POLYGON);
				glVertex3f(0.0f, 0.0f, 0.0f);
				glVertex3f(width, 0.0f, 0.0f);
				glVertex3f(width, itemDim, 0.0f);
				glVertex3f(0.0f, itemDim, 0.0f);
			glEnd();
		}

		float textHeight = itemDim * CHAR_SCALE;
		glTranslatef(textHeight / 2.0f, textHeight / 2.0f, 0.0f);
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

		GLContext *currContext = GLContext::getCurrentContext();
		assert(currContext != NULL);
		currContext->getCurrentContext()->renderString(text, Vector2f(textHeight * text.size(), textHeight), 0.15f);

		glPopMatrix();
	}

	void TextScrollMenu::renderScrollBar(void)
	{
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();

		glColor4f(1.0f, 1.0f, 1.0f, LINE_OPACITY);
		
		float border = itemDim * SCROLL_BAR_BORDER;
		glBegin(GL_LINE_STRIP);
			glVertex3f(width, 0.0f, 0.0f);
			glVertex3f(width + (border * 3.0f), 0.0f, 0.0f);
			glVertex3f(width + (border * 3.0f), height, 0.0f);
			glVertex3f(width, height, 0.0f);
		glEnd();

		float sbHeight = (height * ((float)displayCount / (float)menuItems.size())) - border;
		float sbWidth = border;
		float sbPosn = (sbHeight / displayCount) * index;
		
		glBegin(GL_POLYGON);
			glVertex3f(border + width, border + sbPosn, 0.0f);
			glVertex3f(border + width + sbWidth, border + sbPosn, 0.0f);
			glVertex3f(border + width + sbWidth, border + sbPosn + sbHeight, 0.0f);
			glVertex3f(border + width, border + sbPosn + sbHeight, 0.0f);
		glEnd();

		glPopMatrix();
	}
	
}

