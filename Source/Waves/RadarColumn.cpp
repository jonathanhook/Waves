/**
 * Class:	RadarColumn
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */

#define _USE_MATH_DEFINES
#include <math.h>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include "JDHUtility/GLContext.h"
#include "JDHUtility/GLFontRenderer.h"
#include "JDHUtility/GlutInclude.h"
#include "JDHUtility/LoadImage.h"
#include "InterfaceManager.h"
#include "Playhead.h"
#include "RadarColumn.h"
#include "RadarElement.h"
#include "RadarSegment.h"
#include "WavesControl.h"
#include "WavesGlobalState.h"

using namespace JDHUtility; 

namespace Waves
{
	/* PRIVATE CONSTANTS */
	float		RadarColumn :: HEAD_WIDTH	= 0.01f;
	float		RadarColumn :: HEAD_OPACITY = 0.5f;
	std::string	RadarColumn :: LINE_TEXTURE = "../Images/Controls/dashedLine.tga";
	float		RadarColumn :: NUM_DASHES	= 64.0f;
	float		RadarColumn :: RANGE		= 1.25f;

	/* CONSTRUCTORS */
	RadarColumn::RadarColumn(const Point2f &position, float width, float height) :
		RadarElement(position, width, height, height / 2.0f)
	{

	}

	RadarColumn::~RadarColumn(void)
	{

	}

	/* PUBLIC MEMBER FUNCTIONS */
	void RadarColumn::addControl(WavesControl* control)
	{
		assert(control != NULL);

		float xDiff = control->getPosition().getX() - position.getX();
		float controlXPos = -xDiff;
		control->getSegment().getPosition().setX(controlXPos);

		control->getSegment().setWidth(width);
		control->getSegment().setEnabled(true);
		controls.push_back(control);
	}

	void RadarColumn::fingerAdded(const FingerEventArgs &e)
	{
		assert(modelview != NULL);

		Point2f p = e.getPosition();
		modelview->unTranslatePoint(p);
		modelview->unScalePoint(p);
		lastX = p.getX();

		WavesGlobalState *wgs = WavesGlobalState::getInstance();
		assert(wgs != NULL);

		wgs->getPlayhead().hold();
	}

	void RadarColumn::fingerUpdated(const FingerEventArgs &e)
	{
		assert(modelview != NULL);

		Point2f p = e.getPosition();
		modelview->unTranslatePoint(p);
		modelview->unScalePoint(p);

		float xDiff = lastX - p.getX();	
		WavesGlobalState *wgs = WavesGlobalState::getInstance();
		assert(wgs != NULL);

		wgs->getPlayhead().drag(xDiff);

		lastX = p.getX();
	}

	void RadarColumn::fingerRemoved(const FingerEventArgs &e)
	{
		WavesGlobalState *wgs = WavesGlobalState::getInstance();
		assert(wgs != NULL);

		wgs->getPlayhead().release();
	}

	void RadarColumn::removeControl(WavesControl *control)
	{
		assert(control != NULL);

		control->getSegment().setEnabled(false);
		control->setOnRadar(false);

		for(std::vector<WavesControl *>::iterator it = controls.begin(); it != controls.end();)
		{
			if((*it) == control)
			{
				it = controls.erase(it);
			}
			else it++;
		}
	}

	void RadarColumn::render(void)
	{
		RadarElement::render();

		// render the radar head
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glTranslatef(position.getX(), position.getY(), 0.0f);
		saveTransform();

		glLineWidth(1.0f);
		if(dirty)
		{
			glNewList(displayList, GL_COMPILE);
			glEnable(GL_LINE_SMOOTH);

			renderBackgroundMask();
			
			float labelDim = (width * HEAD_WIDTH) * 4.0f;
			
			glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

			glBegin(GL_LINE_STRIP);
				glVertex3f(-labelDim, getOffsetY(-(labelDim / 3.0f), 0.0f), 0.0f);
				glVertex3f(0.0f, getOffsetY(0.0f, 0.0f), 0.0f);
				glVertex3f(labelDim, getOffsetY(-(labelDim / 3.0f), 0.0f), 0.0f);
			glEnd();

			glBegin(GL_LINE_STRIP);
				glVertex3f(-labelDim, getOffsetY(-(labelDim / 3.0f), 0.0f), 0.0f);
				glVertex3f(-labelDim, getOffsetY(-(labelDim * 1.25f), 0.0f), 0.0f);
				glVertex3f(labelDim, getOffsetY(-(labelDim * 1.25f), 0.0f), 0.0f);
				glVertex3f(labelDim, getOffsetY(-(labelDim / 3.0f), 0.0f), 0.0f);
			glEnd();
			
			// draw the line
			glEnable(GL_TEXTURE_2D);

			GLContext *currContext = GLContext::getCurrentContext();
			assert(currContext != NULL);
			
			currContext->bindTexture(LINE_TEXTURE);

			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		
			glColor4f(1.0f, 1.0f, 1.0f, HEAD_OPACITY);
			glBegin(GL_QUADS);
				glTexCoord2f(0.0f, NUM_DASHES); glVertex3f(-(width * HEAD_WIDTH), getOffsetY(0.0f, 0.0f), 0.0f);
				glTexCoord2f(1.0f, NUM_DASHES); glVertex3f(width * HEAD_WIDTH, getOffsetY(0.0f, 0.0f), 0.0f);
				glTexCoord2f(1.0f, 0.0f);		glVertex3f(width * HEAD_WIDTH, getOffsetY(height, 0.0f), 0.0f);
				glTexCoord2f(0.0f, 0.0f);		glVertex3f(-(width * HEAD_WIDTH), getOffsetY(height, 0.0f), 0.0f);
			glEnd();
			glDisable(GL_TEXTURE_2D);

			// draw the divisions
			WavesGlobalState *wgs = WavesGlobalState::getInstance();
			assert(wgs != NULL);

			float playheadPosition = wgs->getPlayhead().getPosition();
			float playheadRot = playheadPosition * (2.0f * M_PI);
			float radius = width / 2.0f;
			float increment = InterfaceManager::getTrackHeight() + BORDER_DIM;

			// horizontal divisions
			for(float f = increment; f < height; f += increment)
			{
				glBegin(GL_LINE_LOOP);
					for(unsigned int i = 0; i < CIRCLE_VERTICES; i++)
					{
						float offset = (i * -((2.0f * M_PI) / (float)CIRCLE_VERTICES)) + playheadRot;
						float vx = cos(offset) * radius;
						float vz = sin(offset) * radius;

						float zDist = fabs(vz - radius) / width;
						float topY = getOffsetY(f, zDist);
						float alpha = 1.0f - zDist;
		
						glColor4f(1.0f, 1.0f, 1.0f, alpha * (LINE_OPACITY * 0.5f));
						glVertex3f(vx, topY, vz);
					}
				glEnd();
			}

			// vertical divisions
			for(unsigned int i = 0; i < InterfaceManager::BEATS; i += 4)
			{
				float theta = (i * ((2.0f * M_PI) / (float) InterfaceManager::BEATS)) + playheadRot;
				float vx = cos(theta) * radius;
				float vz = sin(theta) * radius;
				float zDist = fabs(vz - radius) / width;
				float topY = getOffsetY(0.0f, zDist);
				float baseY = getOffsetY(height, zDist);
				float alpha = 1.0f - zDist;

				glColor4f(1.0f, 1.0f, 1.0f, alpha * (LINE_OPACITY * 0.5f));
				glBegin(GL_LINE_STRIP);
					glVertex3f(vx, topY, vz);
					glVertex3f(vx, baseY, vz);
				glEnd();
			}

			glColor4f(1.0f, 1.0f, 1.0f, 0.5f);

			glBegin(GL_LINE_LOOP);
				for(int i = 0; i < CIRCLE_VERTICES; i++)
				{
					float offset = (i * -((2.0f * M_PI) / (float)CIRCLE_VERTICES)) + playheadRot;
					float vx = cos(offset) * radius;
					float vz = sin(offset) * radius;

					float zDist = fabs(vz - radius) / width;
					float topY = getOffsetY(0.0f, zDist);
					float alpha = 1.0f - zDist;
		
					glColor4f(1.0f, 1.0f, 1.0f, alpha * LINE_OPACITY);
					glVertex3f(vx, topY, vz);
				}
			glEnd();
		
			glBegin(GL_LINE_LOOP);
				for(int i = 0; i < CIRCLE_VERTICES; i++)
				{
					float offset = (i * -((2.0f * M_PI) / (float)CIRCLE_VERTICES)) + playheadRot;
					float vx = cos(offset) * radius;
					float vz = sin(offset) * radius;

					float zDist = fabs(vz - radius) / width;
					float baseY = getOffsetY(height, zDist);
					float alpha = 1.0f - zDist;
		
					glColor4f(1.0f, 1.0f, 1.0f, alpha * LINE_OPACITY);
					glVertex3f(vx, baseY, vz);
				}
			glEnd();
				
			glDisable(GL_LINE_SMOOTH);
			glEndList();
		}
		glCallList(displayList);
		
		renderInterfaceElements();

		// draw the text
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();

		float textDim = BAR_DIM * 0.75f;
		float textHeight = textDim;
		float textWidth = textDim * 4.0f;

		glTranslatef(-(textDim * 2.0f), getOffsetY(-textDim * 2.25f, 0.0f), 0.0f);

		glPushAttrib(GL_CURRENT_BIT | GL_LINE_BIT);
		glLineWidth(1.0f);
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

		WavesGlobalState *wgs = WavesGlobalState::getInstance();
		assert(wgs != NULL);

		float playheadPosition = wgs->getPlayhead().getPosition() * InterfaceManager::BEATS;
		char s[32];
		sprintf(s, (playheadPosition < 10)?"0%2.1f":"%2.1f", playheadPosition);

		glLineWidth(1.0f);
		GLContext *currContext = GLContext::getCurrentContext();
		assert(currContext != NULL);
		currContext->renderString(s, Vector2f(textWidth, textHeight));

		glPopAttrib();
		glPopMatrix();

		glPopMatrix();
	}

	void RadarColumn::sortSegments(void)
	{
		sort(controls.begin(), controls.end(), &WavesControl::compare);

		for(unsigned int i = 0; i < controls.size(); i++)
		{
			WavesControl *control = controls[i];
			assert(control != NULL);

			Point2f &controlPosition = control->getPosition();

			// snap X to the side of the radar
			controlPosition.setX((position.getX() + (width / 2.0f)));
			
			float xDiff = controlPosition.getX() - position.getX();
			float controlXPos = -xDiff;
			
			RadarSegment &radarSegment = control->getSegment();
			Point2f &radarSegmentPosition = radarSegment.getPosition();
			radarSegmentPosition.setX(controlXPos);
			
			// snap Y to nearest grid
			float y = controlPosition.getY();
			float gridAbove = fmodf(y, InterfaceManager::getTrackHeight() + BORDER_DIM);

			// snap to either the one above, or below
			if(gridAbove > (InterfaceManager::getTrackHeight() + BORDER_DIM) / 2.0f)
			{
				float ny = y + ((InterfaceManager::getTrackHeight() + BORDER_DIM) - gridAbove);
				control->getPosition().setY(ny);
			}
			else
			{
				float ny = y - gridAbove;
				control->getPosition().setY(ny);
			}
			
			// sort Y
			for(unsigned int j = (i + 1); j < controls.size(); j++)
			{
				WavesControl *comparisonControl = controls[j];
				assert(comparisonControl != NULL);

				if((comparisonControl->getPosition().getY() >= control->getPosition().getY()) && (comparisonControl->getPosition().getY() < (control->getPosition().getY() + control->getHeight())))
				{
					comparisonControl->getPosition().setY(control->getPosition().getY() + control->getHeight());
				}
			}
		}
	}

	void RadarColumn::updateSegments(WavesControl *control)
	{
		assert(control != NULL);

		if(!control->isOnRadar() && controlInRange(control))
		{
			control->setOnRadar(true);
			addControl(control);
		}
		else if(control->isOnRadar() &&  controlInRange(control))
		{
			float cy = control->getPosition().getY();
			float cHeight = control->getHeight();
			if(cy <= position.getY())
			{
				cy = position.getY();
			}
			else if(cy + cHeight > height)
			{
				cy = height - cHeight;
			}

			float xDiff = control->getPosition().getX() - position.getX();
			float controlXPos = -xDiff;
			control->getSegment().getPosition().setX(controlXPos);
		}
		else if(control->isOnRadar() && !controlInRange(control))
		{
			removeControl(control);
		}
	}


	/* PRIVATE MEMBER FUNCTIONS */
	bool RadarColumn :: controlInRange(WavesControl *control)
	{
		assert(control != NULL);

		float cx = control->getPosition().getX();
		float wx = control->getPosition().getX() + control->getWidth();

		return (cx >= position.getX() - ((width * RANGE)  / 2.0f) && cx <= position.getX() + ((width * RANGE) / 2.0f)) || (wx >= position.getX() - ((width * RANGE)  / 2.0f) && wx <= position.getX() + ((width * RANGE) / 2.0f));
	}

	void RadarColumn :: renderBackgroundMask(void)
	{
		glPushAttrib(GL_CURRENT_BIT);

		float radius = width / 2.0f;
		glColor4f(0.0f, 0.0f, 0.0f, MASK_OPACITY);
		glBegin(GL_TRIANGLE_STRIP);
			for(int i = 0; i <= CIRCLE_VERTICES; i++)
			{
				float offset = M_PI + (i * -(M_PI / (float)CIRCLE_VERTICES));
				float vx = cos(offset) * radius;
				float vz = sin(offset) * radius;

				float zDist = fabs(vz - radius) / width;
				float topY = getOffsetY(0.0f, zDist);
				float baseY = getOffsetY(height, zDist);
				float alpha = 1.0f - zDist;

				glVertex3f(vx, topY, vz);
				glVertex3f(vx, baseY, vz);
			}
		glEnd();

		glPopAttrib();
	}
}
