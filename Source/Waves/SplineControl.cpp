/**
 * Class:	SplineControl
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */
#include <algorithm>
#include <vector>
#include "ControlPoint.h"
#include "FingerHeldCursor.h"
#include "JDHUtility/GLContext.h"
#include "InterfaceManager.h"
#include "JDHUtility/Interpolator.h"
#include "ReplicateRegion.h"
#include "SplineControl.h"
#include "SplinePreset.h"
#include "WavesGlobalState.h"

using namespace JDHUtility;

namespace Waves
{
	/* PROTECTED CONSTANTS */
	const float SplineControl::INITIAL_VALUE = 0.5f;
	const float SplineControl::LEGEND_DIM = 0.1f;
	const float SplineControl::LEGEND_SUBBEAT_DIM = 0.5f;
	const float SplineControl::UPSAMPLE_FACTOR = 4;

	/* PRIVATE CONSTANTS */
	const float SplineControl::CONTROL_POINT_SIZE = 0.15f;
	const float SplineControl::FINGER_RANGE_FACTOR = 0.15f;
	const float SplineControl::HELD_CURSOR_SIZE = 0.075f;
	const float SplineControl::NEW_CURSOR_MOVEMENT_THRESHOLD = 0.01f;
	const float SplineControl::RANGE = 0.2f;
	const float SplineControl::TEXT_OPACITY = 0.75f;

	/* CONSTRUCTORS */
	SplineControl::SplineControl(const Point2f &position, float width, float height, bool presetSaved) :
		InterfaceElement(position, width, height)
	{
		this->presetSaved = presetSaved;

		exited					= NULL;
		fullLengthDisplayList	= glGenLists(1);
		fullLengthDirty			= true;
		splineDirty				= true;
		splineDisplayList		= glGenLists(1);
	}

	SplineControl::~SplineControl(void)
	{

	}

	/* PUBLIC MEMBER FUNCTIONS */
	bool SplineControl::contains(const FingerEventArgs &e) const
	{
		assert(modelview != NULL);

		Point2f p =	e.getPosition();
		modelview->unTranslatePoint(p);

		float legendDim = (height * LEGEND_DIM) * modelview->getYScale();

		return p.getX() >= 0.0f &&
			p.getX() <= (width * modelview->getXScale()) &&
			p.getY() >= legendDim &&
			p.getY() <= (height * modelview->getYScale()) - legendDim;
	}

	bool SplineControl::controlPointContains(const FingerEventArgs &e) const
	{
		assert(modelview != NULL);

		Point2f p =	e.getPosition();
		modelview->unTranslatePoint(p);

		// use legend dim on left and right as a buffer to prevent deletion near edge
		float legendDim = (height * LEGEND_DIM) * modelview->getYScale();

		return p.getX() >= -legendDim &&
			p.getX() <= (width * modelview->getXScale()) + legendDim &&
			p.getY() >= 0.0f &&
			p.getY() <= height * modelview->getYScale();
	}

	void SplineControl::exitButton_Clicked(void)
	{
		if(exited != NULL) exited();
	}

	void SplineControl::fingerAdded(const FingerEventArgs &e)
	{	
		assert(modelview != NULL);

		Point2f p = e.getPosition();
		modelview->unTranslatePoint(p);
		modelview->unScalePoint(p);

		if(heldFingers.count(e.getId()) == 0)
		{
			HeldFinger h;
			h.currentPosition = p;
			h.downPosition = p;
			h.downTime = glutGet(GLUT_ELAPSED_TIME);
			h.heldCursor = NULL;

			heldFingers[e.getId()] = h;
		}
	}

	void SplineControl::fingerUpdated(const FingerEventArgs &e)
	{
		assert(modelview != NULL);

		Point2f p = e.getPosition();
		modelview->unTranslatePoint(p);
		modelview->unScalePoint(p);

		if(heldFingers.count(e.getId()) > 0)
		{
			HeldFinger *h = &heldFingers[e.getId()];
			assert(h != NULL);

			h->currentPosition = p;

			float dx = fabs(h->currentPosition.getX() - h->downPosition.getX());
			float dy = fabs(h->currentPosition.getY() - h->downPosition.getY());
			float dist = sqrt((dx * dx) + (dy * dy));

			if(dist >= NEW_CURSOR_MOVEMENT_THRESHOLD)
			{
				if (h->heldCursor != NULL) deleteInterfaceElement(h->heldCursor);
				heldFingers.erase(e.getId());
			}
		}
	}

	void SplineControl::fingerRemoved(const FingerEventArgs &e)
	{
		if(heldFingers.count(e.getId()) > 0)
		{
			HeldFinger *h = &heldFingers[e.getId()];
			assert(h != NULL);

			if(h->heldCursor != NULL)
			{
				deleteInterfaceElement(h->heldCursor);
			}

			heldFingers.erase(e.getId());
		}
	}

	vector<ControlPoint*> SplineControl::getControlPoints(void)
	{
		return controlPoints;
	}

	unsigned int SplineControl::getPresetId(void) const
	{
		return presetId;
	}

	bool SplineControl::isPresetSaved(void) const
	{
		return presetSaved;
	}

	void SplineControl::reset(void)
	{
		for(std::vector<ControlPoint *>::iterator it = controlPoints.begin(); it != controlPoints.end(); it++)
		{
			deleteInterfaceElement(*it);
		}

		presetSaved = false;
		presetId = 0;
		controlPoints.clear();
		dirty = true;
	}

	void SplineControl::registerControlPoint(const Point2f &p)
	{
		ControlPoint *controlPoint = new ControlPoint(p, height * CONTROL_POINT_SIZE);
		controlPoint->setFingerDragged(MakeDelegate(this, &SplineControl::controlPoint_FingerDragged));
		controlPoint->setFingerUp(MakeDelegate(this, &SplineControl::controlPoint_FingerUp));	
		registerInterfaceElement(controlPoint);
	
		controlPoints.push_back(controlPoint);
		sort(controlPoints.begin(), controlPoints.end(), &ControlPoint::compare);

		fullLengthDirty = true;
		dirty = true;
		splineDirty = true;
	}

	void SplineControl::render(void)
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
				glVertex3f(0.0f, 0.0f, 0.0f);
				glVertex3f(width, 0.0f, 0.0f);
				glVertex3f(width, height, 0.0f);
				glVertex3f(0.0f, height, 0.0f);
			glEnd();

			glColor4f(1.0f, 1.0f, 1.0f, 0.33f);

			// centre line
			glBegin(GL_LINE_STRIP);
				glVertex3f(0.0f, height / 2.0f, 0.0f);
				glVertex3f(width, height / 2.0f, 0.0f);
			glEnd();

			float legendDim = height * LEGEND_DIM;

			// divisions
			for(unsigned int i = 0; i < InterfaceManager::BEATS; i++)
			{
				float pos = InterfaceManager::getBeatLength() * (float)i;
				if(pos > width)
				{
					break;
				}
				else
				{
					glBegin(GL_LINE_STRIP);
						glVertex3f(pos, legendDim, 0.0f);
						glVertex3f(pos, height - legendDim, 0.0f);
					glEnd();
				}
			}

			// legends
			glColor4f(1.0f, 1.0f, 1.0f, LINE_OPACITY);
			glBegin(GL_LINES);
				glVertex3f(0.0f, legendDim, 0.0f);
				glVertex3f(width, legendDim, 0.0f);
				glVertex3f(0.0f, height - legendDim, 0.0f);
				glVertex3f(width, height - legendDim, 0.0f);
			glEnd();
			
			float xIncrement = InterfaceManager::getBeatLength() / (float)LEGEND_BEAT_SUBDIVISIONS;
			for(unsigned int i = 0; xIncrement * (float)i < width; i++)
			{
				float lineLength = legendDim;
				if((i % LEGEND_BEAT_SUBDIVISIONS) != 0)
				{
					lineLength *= LEGEND_SUBBEAT_DIM;
				}

				glBegin(GL_LINES);
					// top
					glVertex3f(xIncrement * i, legendDim, 0.0f);
					glVertex3f(xIncrement * i, legendDim - lineLength, 0.0f);

					// bottom
					glVertex3f(xIncrement * i, height - legendDim, 0.0f);
					glVertex3f(xIncrement * i, (height - legendDim) + lineLength, 0.0f);
				glEnd();
			}

			glEndList();
			dirty = false;
		}
		glCallList(displayList);

		glLineWidth(getLineWidth(1.0f));
		if(splineDirty)
		{
			glNewList(splineDisplayList, GL_COMPILE);
			renderSplineControl(width);
			glEndList();

			splineDirty = false;
		}
		glCallList(splineDisplayList);

		renderInterfaceElements();

		glPopAttrib();
		glPopMatrix();
	
	}

	void SplineControl::setPresetId(unsigned int presetId)
	{
		this->presetId = presetId;
	}

	void SplineControl::setPresetSaved(bool presetSaved)
	{
		this->presetSaved = presetSaved;
	}

	void SplineControl::setExitedCallback(ExitedCallback exited)
	{
		this->exited = exited;
	}

	/* PROTECTED MEMBER FUNCTIONS */
	float SplineControl::constrainXVal(float val)
	{
		if(val < 0.0f) val = 0.0f;
		if(val > width) val = width;
		return val;
	}

	float SplineControl::constrainYVal(float val)
	{
		if(val > 1.0f) val = 1.0f;
		else if(val < 0.0f) val = 0.0f;

		assert(val >= 0.0f && val <= 1.0f); 

		return val;
	}

	float SplineControl::getValueFromControlPoint(ControlPoint &cp)
	{
		float legendHeight = height * LEGEND_DIM;
		float legendlessHeight = height - (legendHeight * 2.0f);
		float cpy = 1.0f - ((cp.getPosition().getY() - legendHeight) / legendlessHeight);
		cpy = constrainYVal(cpy);

		return cpy;
	}
	/*
	void SplineControl::renderSplineControl(float SplineControlWidth)
	{
		glPushAttrib(GL_LINE_BIT | GL_CURRENT_BIT);
		
		glColor4f(1.0f, 1.0f, 1.0, 1.0f);
		glLineWidth(getLineWidth(2.0f));
		
		glEnable(GL_LINE_SMOOTH);
		glBegin(GL_LINE_STRIP);
			float increment = InterfaceManager::getBeatLength() / (float)InterfaceManager::VALUES_PER_BEAT;
			for(float f = 0.0; f <= SplineControlWidth; f+= (increment / (float)UPSAMPLE_FACTOR))
			{		
				float legendlessHeight = height - ((height * LEGEND_DIM) * 2.0f);
				float value = getValue(f);
				float vx = f;
				float vy = (value * legendlessHeight) + (height * LEGEND_DIM);
				glVertex3f(vx, height - vy, 0.0f);
			}
		glEnd();
		glDisable(GL_LINE_SMOOTH);

		glPopAttrib();
	}
	*/
	void SplineControl::savePreset(unsigned int presetType)
	{
		WavesGlobalState *wavesGlobalState = WavesGlobalState::getInstance();
		assert(wavesGlobalState != NULL);

		std::map<unsigned int, SplinePreset *> &splinePresets = wavesGlobalState->getSplinePresets();

		unsigned int numControlPoints = controlPoints.size();
		if(numControlPoints > 0)
		{
			std::vector<const Point2f> points;
			for(unsigned int i = 0; i < numControlPoints; i++)
			{
				ControlPoint *cp = controlPoints[i];
				assert(cp != NULL);

				const Point2f &cPos = cp->getPosition();
				
				float sx = cPos.getX() / width;
				float sy = cPos.getY() / height;
				Point2f sp(sx, sy);
				points.push_back(sp);
			}	
			
			// HACK : assume that if saved and Id in particular range, it must be a default and so save an altered copy
			if((!presetSaved && numControlPoints > 0) || (presetSaved && presetId >= 0 && presetId <= 5))
			{
				unsigned int pId = SplinePreset::generateId();
				
				SplinePreset *preset = new SplinePreset(pId, points, width, (SplinePreset::PresetType)presetType);	
				assert(preset != NULL);

				splinePresets[pId] = preset;
				
				preset->save();
				presetId = pId;
				presetSaved = true;
			}
			else
			{				
				SplinePreset *preset = splinePresets[presetId];
				assert(preset != NULL);

				preset->setPoints(points);
				preset->save();
			}
		}
	}

	/* PRIVATE MEMBER FUNCTIONS */
	void SplineControl::checkForAddGesture(void)
	{
		for(std::map<unsigned int, HeldFinger>::iterator it = heldFingers.begin(); it != heldFingers.end(); it++)
		{
			HeldFinger *h = &(*it).second;
			assert(h != NULL);

			if(h->heldCursor == NULL)
			{
				float elapsed = glutGet(GLUT_ELAPSED_TIME) - h->downTime;

				if(elapsed >= NEW_CURSOR_CREATE_DELAY)
				{
					h->heldCursor = new FingerHeldCursor(h->downPosition, HELD_CURSOR_SIZE, NEW_CURSOR_PERIOD);
					h->heldCursor->setFingerHeldCallback(MakeDelegate(this, &SplineControl::heldFinger_FingerHeld));
					registerInterfaceElement(h->heldCursor);
				}
			}
		}

		
	}

	void SplineControl::controlPoint_FingerDragged(ControlPoint &cp, const FingerEventArgs &e)
	{
		assert(modelview != NULL);

		Point2f p = e.getPosition();
		modelview->unTranslatePoint(p);
		modelview->unScalePoint(p);

		cp.setPosition(p);
		sort(controlPoints.begin(), controlPoints.end(), &ControlPoint::compare);
		cp.setInside(controlPointContains(e));
	
		fullLengthDirty = true;
		dirty = true;
		splineDirty = true;
	}

	void SplineControl::controlPoint_FingerUp(ControlPoint &cp, const FingerEventArgs &e)
	{
		if(!controlPointContains(e))
		{
			for(vector<ControlPoint *>::iterator it = controlPoints.begin(); it != controlPoints.end(); it++)
			{
				ControlPoint *itCp = *it;
				assert(itCp != NULL);

				if(cp.getId() == itCp->getId()) 
				{
					controlPoints.erase(it);
					break;
				}
			}

			deleteInterfaceElement(&cp);
		}
		
		snapControlPoint(cp);

		fullLengthDirty = true;
		dirty = true;
		splineDirty = true;
	}

	void SplineControl::heldFinger_FingerHeld(FingerHeldCursor *fingerHeld)
	{
		Point2f addPos;
		unsigned int id = 0; 

		// erase the cursor
		for(std::map<unsigned int, HeldFinger>::iterator it = heldFingers.begin(); it != heldFingers.end(); )
		{
			HeldFinger *h = &(*it).second;
			assert(h != NULL);

			if(h->heldCursor == fingerHeld)
			{
				addPos = h->downPosition;
				id = (*it).first;

				deleteInterfaceElement(h->heldCursor);
				it = heldFingers.erase(it);
			}
			else it++;
		}

		// add the new control point
//		if(isCaptured())
//		{
//			MultiTouchEventManager *capturer = getCapturer();
//			capturer->release(id);
//		}

		registerControlPoint(addPos);
	}

	bool SplineControl::isClose(const Point2f &p)
	{
		float val = getValue(p.getX());
		float distance = fabs((p.getY() / height) - (1.0f - val));

		return distance < (1.0f * FINGER_RANGE_FACTOR);
	}

	void SplineControl::snapControlPoint(ControlPoint &cp)
	{
		float cpx = cp.getPosition().getX();
		float cpy = cp.getPosition().getY();

		// clamp x
		if(cpx < 0.0f) cpx = 0.0f;
		else if(cpx > width) cpx = width;

		// clamp y
		float legendDim = (height * LEGEND_DIM);
		if(cpy < legendDim) cpy = legendDim;
		else if(cpy > height - legendDim) cpy = (height - legendDim);

		// snap
		float snapIncrement = InterfaceManager::getBeatLength() / (float)BEAT_SNAP;
		float divs = floorf((cpx + (snapIncrement * 0.5f)) / snapIncrement);
		cpx = snapIncrement * divs;

		cp.getPosition().setX(cpx);
		cp.getPosition().setY(cpy);
	}

	void SplineControl::update(void)
	{
		InterfaceElement::update();
		checkForAddGesture();
	}
}