/**
 * Class:	GridScrollMenuItem
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */
#include "JDHUtility/GLContext.h"
#include "JDHUtility/GlutInclude.h"
#include "GridScrollMenuItem.h"
#include "Visuals/Visual.h"

using namespace JDHUtility;

namespace Waves
{
	/* PRIVATE CONSTANTS */
	const float GridScrollMenuItem::TEXT_DIM = 0.125f;

	/* CONSTRUCTORS */
	GridScrollMenuItem::GridScrollMenuItem(const Point2f &position, float width, float height, Visual *visual) :
		Button(position, width, height)
	{
		this->visual = visual;

		assert(visual != NULL);
		textLabel = visual->getName();

		GridScrollMenuItemDirty = true;
		GridScrollMenuItemDisplayList = glGenLists(1);
	}

	GridScrollMenuItem::~GridScrollMenuItem(void)
	{
		if(visual != NULL) delete visual;
		visual = NULL;
	}

	/* PUBLIC MEMBER FUNCTIONS */
	void GridScrollMenuItem::fingerRemoved(const FingerEventArgs &e)
	{
		Button::fingerRemoved(e);

		if(visualSelected != NULL && contains(e))
		{
			visualSelected(visual);
		}
	}

	void GridScrollMenuItem::render(void)
	{
		Button::render();

		assert(visual != NULL);

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		
		glTranslatef(position.getX(), position.getY(), 0.0f);
		saveTransform();

		// thumbnail
		unsigned int thumbnailTexture = visual->renderThumbnail(true);
		glEnable(GL_TEXTURE_2D);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glBindTexture(GL_TEXTURE_2D, thumbnailTexture);

		glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(0.0f, 0.0f, 0.0f);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(width, 0.0f, 0.0f);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(width, height, 0.0f);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(0.0f, height, 0.0f);
		glEnd();

		glDisable(GL_TEXTURE_2D);

		glLineWidth(1.0f);
		if(GridScrollMenuItemDirty)
		{
			glNewList(GridScrollMenuItemDisplayList, GL_COMPILE);
			
			glBegin(GL_LINE_LOOP);
				glVertex3f(0.0f, 0.0f, 0.0f);
				glVertex3f(width, 0.0f, 0.0f);
				glVertex3f(width, height, 0.0f);
				glVertex3f(0.0f, height, 0.0f);
			glEnd();

			glMatrixMode(GL_MODELVIEW);
			glPushMatrix();

			float textHeight = height * TEXT_DIM;

			glTranslatef(textHeight / 3.0f, textHeight / 3.0f, 0.0f);
			glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

			unsigned int textLength = textLabel.size();

			GLContext *currContext = GLContext::getCurrentContext();
			assert(currContext != NULL);
			currContext->renderString(textLabel, Vector2f(textLength * textHeight, textHeight), BORDER_DIM);
			
			glPopMatrix();

			glEndList();
			GridScrollMenuItemDirty = false;
		}
		glCallList(GridScrollMenuItemDisplayList);
		
		glPopMatrix();
	}

	void GridScrollMenuItem::setVisualSelected(VisualSelectedCallback visualSelected)
	{
		this->visualSelected = visualSelected;
	}

	void GridScrollMenuItem::update(void)
	{
		assert(visual != NULL);

		InterfaceElement::update();
		visual->update();
	}
}