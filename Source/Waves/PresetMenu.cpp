/**
 * Class:	PresetMenu
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */
#include "JDHUtility/GlutInclude.h"
#include "InterfaceManager.h"
#include "JDHUtility/Interpolator.h"
#include "PresetMenu.h"

namespace Waves
{
	/* CONSTRUCTORS */
	PresetMenu::PresetMenu(const Point2f &position, float height, std::vector<SplinePreset *> menuItems) :
		ScrollMenu(position, height, height, ScrollMenu::HORIZONTAL, menuItems.size())
	{
		this->menuItems = menuItems;
		updateDisplayCount();
	}

	PresetMenu::~PresetMenu(void)
	{
	}
	
	/* PUBLIC MEMBER FUNCTIONS */
	void PresetMenu::fingerRemoved(const FingerEventArgs &e)
	{
		// if not moved then click
		if(selected && fingerDown && !moved)
		{
			if(itemSelected != NULL) itemSelected(menuItems[selectedIndex + index]);
		}
		
		ScrollMenu::fingerRemoved(e);
	}
	
	void PresetMenu::render(void)
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
			glBegin(GL_LINE_LOOP);
				glVertex3f(0.0f, height, 0.0f);
				glVertex3f(0.0f, 0.0f, 0.0f);
				glVertex3f(width, 0.0f, 0.0f);
				glVertex3f(width, height, 0.0f);
			glEnd();

			renderScrollBar();

			for(unsigned int i = index; i < (displayCount + index); i++)
			{
				renderMenuItem(menuItems[i], (selected && (i - index == selectedIndex)));
				glTranslatef(itemDim, 0.0f, 0.0f);
			}
			
			glDisable(GL_TEXTURE_2D);
			glEndList();

			dirty = false;
		}
		glCallList(displayList);

		glPopAttrib();
		glPopMatrix();
	}

	void PresetMenu::setItemSelectedCallback(ItemSelectedCallback itemSelected)
	{
		this->itemSelected = itemSelected;
	}

	void PresetMenu::setMenuItems(std::vector<SplinePreset *> menuItems)
	{
		this->menuItems = menuItems;
		updateDisplayCount();
	}

	/* PRIVATE MEMBER FUNCTIONS */
	void PresetMenu::renderMenuItem(SplinePreset *preset, bool selected)
	{
		assert(preset != NULL);
		// TODO : DL IT!

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();

		float border = ((itemDim * SCROLL_BAR_BORDER) * 0.25f) * 3.0f;

		glColor4f(1.0f, 1.0f, 1.0f, LINE_OPACITY);
		glBegin(GL_LINE_STRIP);
			glVertex3f(itemDim, 0.0f, 0.0f);
			glVertex3f(itemDim, height - border, 0.0f);
		glEnd();

		if(selected)
		{
			glColor4f(1.0f, 1.0f, 1.0f, 0.25f);
			glBegin(GL_POLYGON);
				glVertex3f(0.0f, 0.0f, 0.0f);
				glVertex3f(itemDim, 0.0f, 0.0f);
				glVertex3f(itemDim, height - border, 0.0f);
				glVertex3f(0.0f, height - border, 0.0f);
			glEnd();
		}

		// LINE
		std::vector<const Point2f> points = preset->getPoints();
		unsigned int pointCount = points.size();
		float increment = width / (float)pointCount;

		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		glEnable(GL_LINE_SMOOTH);
		glBegin(GL_LINE_STRIP);		
		// first
		float x1 = points[0].getX();
		float inc = x1;
		
		// HACK : not exactly right but its only a fooking menu!
		for(float f = 0; f < 1.0f; f+= 0.1f)
		{
			float y0 = points[(pointCount - 2) % pointCount].getY();
			float y1 = points[(pointCount - 1) % pointCount].getY();
			float y2 = points[0].getY();
			float y3 = points[1 % pointCount].getY();

			float y = Interpolator::hermiteInterpolate(y0, y1, y2, y3, f, Interpolator::DEFAULT_TENSION, 0.0f);	
			y = scaleY(y);

			glVertex3f((inc * f) * itemDim, y, 0.0f);
		}
		
		// middle sections
		for(unsigned int i = 0; i < pointCount - 1; i++)
		{
			float x0 = points[i].getX();
			float x1 = points[(i + 1) % pointCount].getX();
			float increment = x1 - x0;

			for(float f = 0; f < 1.0f; f+= 0.1f)
			{
				float y0 = points[(i - 1) % pointCount].getY();
				float y1 = points[i % pointCount].getY();
				float y2 = points[(i + 1) % pointCount].getY();
				float y3 = points[(i + 2) % pointCount].getY();

				float y = Interpolator::hermiteInterpolate(y0, y1, y2, y3, f, Interpolator::DEFAULT_TENSION, 0.0f);	
				y = scaleY(y);

				glVertex3f((x0 + (increment * f)) * itemDim, y, 0.0f);
			}
		}

		// last
		float x0 = points[pointCount - 1].getX();
		x1 = 1.0f;
		inc = x1 - x0;
		
		// HACK : not exactly right but its only a fooking menu!
		for(float f = 0; f < 1.0f; f+= 0.1f)
		{
			float y0 = points[(pointCount - 2) % pointCount].getY();
			float y1 = points[(pointCount - 1) % pointCount].getY();
			float y2 = points[0].getY();
			float y3 = points[1 % pointCount].getY();

			float y = Interpolator::hermiteInterpolate(y0, y1, y2, y3, f, Interpolator::DEFAULT_TENSION, 0.0f);	
			y = scaleY(y);

			glVertex3f((x0 + (inc * f)) * itemDim, y, 0.0f);
		}
		glVertex3f(itemDim, points[0].getY() * height, 0.0f);

		glEnd();

		glDisable(GL_LINE_SMOOTH);
		// LINE

		glPopMatrix();
	}

	void PresetMenu::renderScrollBar(void)
	{
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();

		glColor4f(1.0f, 1.0f, 1.0f, LINE_OPACITY);
		
		float border = (itemDim * SCROLL_BAR_BORDER) * 0.25f;
		glBegin(GL_LINE_STRIP);
			glVertex3f(0.0f, height - (border * 3.0f), 0.0f);
			glVertex3f(width, height - (border * 3.0f), 0.0f);
			glVertex3f(width, height, 0.0f);
			glVertex3f(0.0f, height, 0.0f);
		glEnd();

		float sbWidth = (width * ((float)displayCount / (float)menuItems.size())) - border;
		float sbHeight = border / 3.0f;
		float sbPosn = (sbWidth / displayCount) * index;
		
		glBegin(GL_POLYGON);
			glVertex3f(border + sbPosn, height - border, 0.0f);
			glVertex3f(sbWidth + sbPosn, height - border, 0.0f);
			glVertex3f(sbWidth + sbPosn, height - (border * 2.0f), 0.0f);
			glVertex3f(border + sbPosn, height - (border * 2.0f), 0.0f);
		glEnd();

		glPopMatrix();
	}

	float PresetMenu::scaleY(float y)
	{
		if(y < 0.0f)
		{
			y = 0.0f;
		}
		else if(y > 1.0f)
		{
			y = 1.0f;
		}

		float border = (itemDim * SCROLL_BAR_BORDER) * 0.25f;
		float dim = height - (border * 5.0f);
		y *= dim;
		y += border;

		return y;
	}

	void PresetMenu::updateDisplayCount(void)
	{
		displayCount = menuItems.size() > PRESET_DISPLAY_COUNT ? PRESET_DISPLAY_COUNT : menuItems.size();
		itemDim = InterfaceManager::getBeatLength() * 4.0f;

		if(menuItems.size() < PRESET_DISPLAY_COUNT)
		{
			width = itemDim * menuItems.size();
		}
		else
		{
			width = itemDim * PRESET_DISPLAY_COUNT;
		}

		itemCount = menuItems.size();
	}

}
