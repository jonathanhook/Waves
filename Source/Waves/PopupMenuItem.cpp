/**
 * Class:	PopupMenuItem
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */
#include "JDHUtility/GlutInclude.h"
#include "PopupMenuItem.h"

namespace Waves
{
	/* STATIC VARS */
	float PopupMenuItem::SELECTED_OPACITY = 0.85f;
	float PopupMenuItem::TEXTURE_OPACITY = 0.85f;

	/* CONSTRUCTORS */
	PopupMenuItem::PopupMenuItem(const Point2f &position, float width, float height) :
		Button(position, width, height)
	{
		clicked = NULL;
		popupMenuItemDisplayList = glGenLists(1);
	}

	PopupMenuItem::~PopupMenuItem(void)
	{

	}

	/* PUBLIC MEMBER FUNCTIONS */
	void PopupMenuItem::fingerRemoved(const FingerEventArgs &e)
	{
		if(clicked != NULL && contains(e))
		{
			//clicked(visual);
		}

		selected = false;
		dirty = true;
	}

	void PopupMenuItem::render(void)
	{
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();

		glTranslatef(position.getX(), position.getY(), 0.0f);
		saveTransform();
	
		glPushAttrib(GL_CURRENT_BIT);

		if(dirty)
		{
			glNewList(popupMenuItemDisplayList, GL_COMPILE);

			if(selected)
			{
				glPolygonOffset(1.0f, 1.0f);
				glColor4f(1.0f, 1.0f, 1.0f, SELECTED_OPACITY);
				glBegin(GL_POLYGON);
					glVertex3f(0.0f, 0.0f, 0.0f);
					glVertex3f(width, 0.0f, 0.0f);
					glVertex3f(width, height, 0.0f);
					glVertex3f(0.0f, height, 0.0f);
				glEnd();		
			}

			glEndList();
			dirty = false;	
		}

		glCallList(popupMenuItemDisplayList);

		glPopAttrib();
		glPopMatrix();
	
	}

	void PopupMenuItem::setClickedCallback(ClickedCallback clicked)
	{
		this->clicked = clicked;
	}
}
