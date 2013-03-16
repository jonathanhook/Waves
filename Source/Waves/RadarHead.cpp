/**
 * Class:	RadarHead
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */
#include <string>
#include "JDHUtility/GLContext.h"
#include "JDHUtility/GlutInclude.h"
#include "InterfaceManager.h"
#include "Playhead.h"
#include "RadarHead.h"
#include "WavesGlobalState.h"

namespace Waves
{
	/* STATIC VARS */
	const float		RadarHead :: MAX_OPACITY = 0.6f;
	const string	RadarHead :: TEXTURE_URI = "../Images/Controls/radarhead.tga";

	/* CONSTRUCTORS */
	RadarHead :: RadarHead(const Point2f &position, float width, float height) :
		InterfaceElement(position, width, height)
	{
	}

	RadarHead :: ~RadarHead(void)
	{
	}

	/* PUBLIC MEMBER FUNCTIONS */
	bool RadarHead :: contains(const FingerEventArgs &e) const
	{
		return false;
	}

	void RadarHead :: fingerAdded(const FingerEventArgs &e)
	{

	}

	void RadarHead :: fingerUpdated(const FingerEventArgs &e)
	{

	}

	void RadarHead :: fingerRemoved(const FingerEventArgs &e)
	{

	}

	void RadarHead :: render(void)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();

		glTranslatef(position.getX(), position.getY(), 0.0f);
		saveTransform();

		// setup texture params
		glEnable(GL_TEXTURE_2D);

		GLContext *currContext = GLContext::getCurrentContext();
		assert(currContext != NULL);
		currContext->bindTexture(TEXTURE_URI);
		
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		WavesGlobalState *wgs = WavesGlobalState::getInstance();
		assert(wgs != NULL);

		float playheadPosition = fmodf(wgs->getPlayhead().getPosition(), width);
		float headWidth = InterfaceManager::getBeatLength();
		float renderWidth = headWidth;
		
		if(playheadPosition < headWidth)
		{
			renderWidth = playheadPosition;

			// draw the wrap around radarhead
			glMatrixMode(GL_MODELVIEW);
			glPushMatrix();
			glTranslatef(width, 0.0f, 0.0f);

			float textureOffset = renderWidth / headWidth;
			glColor4f(1.0f, 1.0f, 1.0f, MAX_OPACITY);
			glBegin(GL_QUADS);
				glTexCoord2f(1.0f - textureOffset, 1.0f);	glVertex3f(0.0f, 0.0f, 0.0f);
				glTexCoord2f(0.0f, 1.0f);					glVertex3f(-(headWidth - renderWidth), 0.0f, 0.0f);
				glTexCoord2f(0.0f, 0.0f);					glVertex3f(-(headWidth - renderWidth), height, 0.0f);
				glTexCoord2f(1.0f - textureOffset, 0.0f);	glVertex3f(0.0f, height, 0.0f);
			glEnd();

			glPopMatrix();
		}
		
		// translate to playhead position
		glTranslatef(playheadPosition, 0.0f, 0.0f);

		float textureOffset = renderWidth / headWidth;
		glColor4f(1.0f, 1.0f, 1.0f, MAX_OPACITY);
		glBegin(GL_QUADS);
			glTexCoord2f(1.0f, 1.0f);					glVertex3f(0.0f, 0.0f, 0.0f);
			glTexCoord2f(1.0f - textureOffset, 1.0f);	glVertex3f(-renderWidth, 0.0f, 0.0f);
			glTexCoord2f(1.0f - textureOffset, 0.0f);	glVertex3f(-renderWidth, height, 0.0f);
			glTexCoord2f(1.0f, 0.0f);					glVertex3f(0.0f, height, 0.0f);
		glEnd();

		glDisable(GL_TEXTURE_2D);

		glPopMatrix();
	
	}
}
