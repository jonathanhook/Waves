/**
 * Class:	DragHandle
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */
#include "DragHandle.h"
#include "JDHUtility/GlutInclude.h"
#include "InterfaceManager.h"
#include "JDHUtility/Interpolator.h"

using namespace JDHUtility;

namespace Waves
{
	/* PRIVATE CONSTANTS */
	const float DragHandle::ICON_POSITION = 0.33f;

	/* CONSTRUCTORS */
	DragHandle::DragHandle(const Point2f &position, float width, float height, HandleType handleType) :
		DragButton(position, width, height)
	{
		this->handleType = handleType;

		dragHandleDirty = true;
		dragHandleDisplayList = glGenLists(1);
	}	

	DragHandle::~DragHandle(void)
	{

	}

	/* PUBLIC MEMBER FUNCTIONS */
	void DragHandle::render(void)
	{
		DragButton::render();

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();

		glTranslatef(position.getX(), position.getY(), 0.0f);
		saveTransform();

		glPushAttrib(GL_CURRENT_BIT | GL_LINE_BIT);

		// STORE PUSH
		// [jdh]: WATCH OUT!
		glLineWidth(getLineWidth(ICON_LINE_WIDTH));
		glPushAttrib(GL_LINE_BIT);
		
		if(dragHandleDirty)
		{
			glNewList(dragHandleDisplayList, GL_COMPILE);
			
			float borderDim = BORDER_DIM * 1.0f;
			float dim = width / 2.0f;

			glEnable(GL_LINE_SMOOTH);

			// triangle
			glTranslatef(0.0f, (height * 0.33f) - (dim * 0.5f), 0.0f);

			glLineWidth(1.0f);

			glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
			for(int i = 0; i < 2; i++)
			{
				glBegin(i == 0 ? GL_POLYGON : GL_LINE_LOOP);
					glVertex3f(dim + (borderDim * 0.5f), borderDim, 0.0f);
					glVertex3f(width - (borderDim * 1.5f), (dim * 0.5f) + borderDim, 0.0f);
					glVertex3f(dim + (borderDim * 0.5f), dim + borderDim, 0.0f);
				glEnd();
			}

			for(int i = 0; i < 2; i++)
			{
				glBegin(i == 0 ? GL_POLYGON : GL_LINE_LOOP);
					glVertex3f(dim - (borderDim * 0.5), borderDim, 0.0f);
					glVertex3f(borderDim * 1.5f, (dim * 0.5f) + borderDim, 0.0f);
					glVertex3f(dim - (borderDim * 0.5), dim + borderDim, 0.0f);
				glEnd();
			}

			// STORE POP
			glPopAttrib();
			glEnable(GL_LINE_SMOOTH);

			// icon
			glTranslatef(0.0f, (height * 0.33f) - (dim * 0.5f), 0.0f);

			if(handleType == EXTEND)
			{
				glBegin(GL_LINES);
					glVertex3f(borderDim * 2.0f, borderDim + (dim * 0.5f), 0.0f);
					glVertex3f(width - (borderDim * 2.0f), borderDim + (dim * 0.5f), 0.0f);

					glVertex3f(width / 2.0f, borderDim, 0.0f);
					glVertex3f(width / 2.0f, dim + borderDim, 0.0f);
				glEnd();
			}
			else
			{
				float points[4] = {0.5f, 0.9f, 0.1f, 0.5f};
				glBegin(GL_LINE_STRIP);

					float xInc = dim / 4.0f;
					for(unsigned int i = 0; i < 4; i++)
					{
						int bi = i - 1;
						if(bi < 0) bi = 0;

						float y0 = points[bi];
						float y1 = points[i % 4];
						float y2 = points[(i + 1) % 4];
						float y3 = points[(i + 2) % 4];

						float x0 = xInc * (float)i;

						for(float mu = 0; mu < 1.0f; mu += 0.1f)
						{
							float y = Interpolator::hermiteInterpolate(y0, y1, y2, y3, mu, Interpolator::HERMITE, 0.0f);
							float x = x0 + (mu * xInc);
							
							glVertex3f((borderDim * 2.0f) + x, borderDim + (y * dim), 0.0f);
						}

					}
				glEnd();
			}

			glDisable(GL_LINE_SMOOTH);

			glEndList();

			dragHandleDirty = false;
		}
		glCallList(dragHandleDisplayList);

		glPopAttrib();
		glPopMatrix();
	}
}