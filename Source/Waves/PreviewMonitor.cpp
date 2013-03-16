/**
 * Class:	PreviewMonitor
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */
#include "JDHUtility/GlutInclude.h"
#include "PreviewMonitor.h"
#include "Visuals/Visual.h"

namespace Waves
{
	/* CONSTRUCTORS */
	PreviewMonitor::PreviewMonitor(const Point2f &position, float width, float height, Visual *visual) :
		InterfaceElement(position, width, height)
	{
		this->visual = visual;
	}

	PreviewMonitor::~PreviewMonitor(void)
	{
	}

	/* PUBLIC MEMBER FUNCTIONS */
	bool PreviewMonitor::contains(const FingerEventArgs &e) const
	{
		return false;
	}

	void PreviewMonitor::fingerAdded(const FingerEventArgs &e)
	{

	}

	void PreviewMonitor::fingerUpdated(const FingerEventArgs &e)
	{
		
	}

	void PreviewMonitor::fingerRemoved(const FingerEventArgs &e)
	{
		
	}

	void PreviewMonitor::render(void)
	{
		assert(visual != NULL);

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glTranslatef(position.getX(), position.getY(), 0.0f);
		saveTransform();

		glPushAttrib(GL_LINE_BIT | GL_CURRENT_BIT);

		// thumbnail
		unsigned int thumbnailTexture = visual->renderThumbnail(false);
		glEnable(GL_TEXTURE_2D);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glBindTexture(GL_TEXTURE_2D, thumbnailTexture);

		float notchDim = BAR_DIM - (BORDER_DIM * 2.0f);
		float notchTexX = notchDim / width;
		float notchTexY = notchDim / height;

		glBegin(GL_POLYGON);
			glTexCoord2f(0.0f, 0.0f);					glVertex3f(0.0f, 0.0f, 0.0f);
			glTexCoord2f(1.0f - notchTexX, 0.0f);		glVertex3f(width - notchDim, 0.0f, 0.0f);
			glTexCoord2f(1.0f - notchTexX, notchTexY);	glVertex3f(width - notchDim, notchDim, 0.0f);
			glTexCoord2f(1.0f, notchTexY);				glVertex3f(width, notchDim, 0.0f);
			glTexCoord2f(1.0f, 1.0f);					glVertex3f(width, height, 0.0f);
			glTexCoord2f(0.0f, 1.0f);					glVertex3f(0.0f, height, 0.0f);
		glEnd();

		/*
		glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(0.0f, 0.0f, 0.0f);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(width, 0.0f, 0.0f);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(width, height, 0.0f);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(0.0f, height, 0.0f);
		glEnd();
		*/
		glDisable(GL_TEXTURE_2D);

		glLineWidth(0.75f);
		if(dirty)
		{
			glNewList(displayList, GL_COMPILE);

			glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
			glBegin(GL_LINE_STRIP);
				glVertex3f(width, BAR_DIM - (BORDER_DIM * 2.0f), 0.0f);
				glVertex3f(width, height, 0.0f);
				glVertex3f(0.0f, height, 0.0f);
				glVertex3f(0.0f, 0.0f, 0.0f);
				glVertex3f(width - (BAR_DIM - (BORDER_DIM * 2.0f)), 0.0f, 0.0f);
			glEnd();

			glEndList();
			dirty = false;
		}
		glCallList(displayList);

		renderInterfaceElements();

		glPopAttrib();
		glPopMatrix();

	}
}
