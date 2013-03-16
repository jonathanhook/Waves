/**
 * Class:	RadarElement
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */
#define _USE_MATH_DEFINES
#include <math.h>
#include "GestureRecogniser.h"
#include "JDHUtility/GlutInclude.h"
#include "InterfaceManager.h"
#include "JDHUtility/Interpolator.h"
#include "Playhead.h"
#include "RadarElement.h"
#include "WavesGlobalState.h"

namespace Waves
{
	/* STATIC VARS */
	const float RadarElement::LINE_OPACITY = 0.8f;
	const float RadarElement::OPACITY = 0.0f;

	/* CONSTRUCTORS */
	RadarElement::RadarElement(const Point2f &position, float width, float height, float vanishingY) :
		InterfaceElement(position, width, height)
	{	
		this->vanishingY = vanishingY;
	
		controlFinger = NULL;
		renderOpacity = OPACITY;
		trackingFinger = false;
	}

	RadarElement::~RadarElement(void)
	{
		for(map<int, Finger *>::iterator it = fingers.begin(); it != fingers.end(); it++)
		{
			if((*it).second != NULL) delete (*it).second;
			(*it).second = NULL;
		}
		fingers.clear();
	}

	/* PUBLIC MEMBER FUNCTIONS */
	bool RadarElement::contains(const FingerEventArgs &e) const
	{
		assert(modelview != NULL);

		Point2f p = e.getPosition();
		modelview->unTranslatePoint(p);
		modelview->unScalePoint(p);

		float x = p.getX();
		float radius = width / 2.0f;
		float z = sqrt((radius * radius) - (x * x));
		z = fabs(z - radius) / width;

		float topY = getOffsetY(0.0f, z);
		float bottomY = getOffsetY(height, z);

		return p.getX() >= -(width * 0.45f) &&
			p.getX() <= (width * 0.45f) &&
			p.getY() >= topY &&
			p.getY() <= bottomY;
	}

	
	void RadarElement::fingerAdded(const FingerEventArgs &e)
	{

	}

	void RadarElement::fingerUpdated(const FingerEventArgs &e)
	{

	}

	void RadarElement::fingerRemoved(const FingerEventArgs &e)
	{

	}

	void RadarElement::render(void)
	{	
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();

		glTranslatef(position.getX(), position.getY(), 0.0f);
		saveTransform();

		float radius = width / 2.0f;
		float ratio = (float)glutGet(GLUT_WINDOW_WIDTH) / (float)glutGet(GLUT_WINDOW_HEIGHT);

		WavesGlobalState *wgs = WavesGlobalState::getInstance();
		assert(wgs != NULL);

		float playheadPosition = wgs->getPlayhead().getPosition();
		float playheadRot = playheadPosition * (2.0f * M_PI);

		glColor4f(1.0f, 1.0f, 1.0f, 0.25f);

		glBegin(GL_TRIANGLE_STRIP);
			for(int i = 0; i <= CIRCLE_VERTICES; i++)
			{
				float offset = (i * -((2.0f * M_PI) / (float)CIRCLE_VERTICES)) + playheadRot;
				float vx = cos(offset) * radius;
				float vz = sin(offset) * radius;

				float zDist = fabs(vz - radius) / width;
				float topY = getOffsetY(0.0f, zDist);
				float baseY = getOffsetY(height, zDist);
				float alpha = 1.0f - zDist;

				glColor4f(1.0f, 1.0f, 1.0f, alpha * renderOpacity);
				float textureX = (float)i / (float)CIRCLE_VERTICES;

				glTexCoord2f(textureX, 1.0f);	glVertex3f(vx, topY, vz);
				glTexCoord2f(textureX, 1.0f - (height * ratio));   glVertex3f(vx, baseY, vz);
			}
		glEnd();

		glDisable(GL_TEXTURE_2D);

		glPopMatrix();
	}

	/* PRIVATE MEMBER FUNCTIONS */
	float RadarElement::getNonOffsetY(const Point2f &p) const
	{
		float x = p.getX();
		float radius = width / 2.0f;
		float z = sqrt((radius * radius) - (x * x));

		//float offset1 = getOffsetY(
		

		return 0;
	}

	float RadarElement::getOffsetY(float yVal, float zVal) const
	{
		// make the actual size at the centre (z)
		zVal -= 0.5f;

		float y0 = yVal + position.getY();
		//float y0 = yVal - position.getY();
		float y1 = vanishingY;
		float mu = zVal / InterfaceManager::VANISHING_POINT_Z;
		return Interpolator::linearInterpolate(y0, y1, mu) - position.getY();
	}
}
