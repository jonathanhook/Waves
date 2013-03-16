/**
 * Class:	GridScrollMenu
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */
#include "JDHUtility/GlutInclude.h"
#include "GridScrollMenu.h"
#include "GridScrollMenuItem.h"
#include "Visuals/Visual.h"

namespace Waves
{
	/* PROTECTED CONSTANTS */
	const float GridScrollMenu::SCROLL_BAR_BORDER = 0.05f;

	/* CONSTRUCTORS */
	GridScrollMenu::GridScrollMenu(const Point2f &position, float width, float height) :
		InterfaceElement(position, width, height)
	{
		itemHeight = height / (float)GRID_DIM;

		float border = itemHeight * SCROLL_BAR_BORDER;
		itemWidth = (width - (border * 3.0f)) / (float)GRID_DIM;
	}

	GridScrollMenu::~GridScrollMenu(void)
	{
	}

	/* PUBLIC MEMBER FUNCTIONS */
	bool GridScrollMenu::contains(const FingerEventArgs &e) const
	{
		return false;
	}

	void GridScrollMenu::fingerAdded(const FingerEventArgs &e)
	{
		
	}
	
	void GridScrollMenu::fingerUpdated(const FingerEventArgs &e)
	{
		
	}
	
	void GridScrollMenu::fingerRemoved(const FingerEventArgs &e)
	{

	}

	void GridScrollMenu::render(void)
	{
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glTranslatef(position.getX(), position.getY(), 0.0f);
		saveTransform();

		glLineWidth(1.0f);
		if(dirty)
		{
			glNewList(displayList, GL_COMPILE);

			glColor4f(1.0f, 1.0f, 1.0f, LINE_OPACITY);
			glBegin(GL_LINE_LOOP);
				glVertex3f(0.0f, 0.0f, 0.0f);
				glVertex3f(width, 0.0f, 0.0f); //Modded 14th
				glVertex3f(width, height, 0.0f);//Modded 14th
				glVertex3f(0.0f, height, 0.0f);
			glEnd();

			renderScrollBar();

			glEndList();
			dirty = false;
		}
		glCallList(displayList);

		renderInterfaceElements();

		glPopMatrix();
	}

	/* PRIVATE MEMBER FUNCTIONS */
	void GridScrollMenu::renderScrollBar(void)
	{
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();

		glLineWidth(1.0f);
		glColor4f(1.0f, 1.0f, 1.0f, LINE_OPACITY);
		
		float border = itemHeight * SCROLL_BAR_BORDER;
		glBegin(GL_LINE_STRIP);
			glVertex3f(width- (border * 3.0f), 0.0f, 0.0f);
			glVertex3f(width- (border * 3.0f), height, 0.0f);
		glEnd();

		float sbHeight = height - (border * 2.0f);
		float sbWidth = border;
		float sbPosn = 0.0f;//(sbHeight) * index;
		
		glBegin(GL_POLYGON);
			glVertex3f(width - (border * 2.0f), border + sbPosn, 0.0f);
			glVertex3f(width - (border * 2.0f) + sbWidth, border + sbPosn, 0.0f);
			glVertex3f(width - (border * 2.0f) + sbWidth, border + sbPosn + sbHeight, 0.0f);
			glVertex3f(width - (border * 2.0f), border + sbPosn + sbHeight, 0.0f);
		glEnd();
		
		glPopMatrix();
	}
}
