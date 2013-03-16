/**
 * Class:	Canvas
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */
#define _USE_MATH_DEFINES
#include <math.h>
#include <string>
#include "Canvas.h"
#include "JDHUtility/GlutInclude.h"
#include "JDHUtility/GLContext.h"
#include "Visuals/ExternalVisual.h"
#include "FastDelegate.h"
#include "FingerHeldCursor.h"
#include "JDHUtility/Colour4f.h"
#include "GestureRecogniser.h"
#include "InterfaceManager.h"
#include "InterfaceElement.h"
#include "JDHUtility/Point2f.h"
#include "RadarColumn.h"
#include "RadarHead.h"
#include "PopupMenu.h"
#include "JDHUtility/Vector2f.h"
#include "VisualWavesControl.h"
#include "WavesControl.h"
#include "WavesGlobalState.h"

using namespace fastdelegate;
using namespace JDHUtility;
using namespace WavesVisuals;

namespace Waves
{
	/* STATIC VARS */
	const float Canvas::HELD_THRESHOLD = 0.01f;
	const float Canvas::HELD_CURSOR_SIZE = 0.075f;
	const float Canvas::INITIAL_SCALE = 1.0f;
	const float Canvas::MAX_SCALE = 3.0f;
	const float Canvas::MENU_DIM = 0.3f;
	const float Canvas::OPACITY = 0.33f;
	const float Canvas::OUTER_REGION = 0.15f;
	const float Canvas::POPUP_SIZE = 0.1f;
	const float Canvas::RADAR_HEAD_SIZE = 8.0f;

	/* CONSTRUCTORS */
	Canvas::Canvas(const Point2f &position, float width, float height) :
		InterfaceElement(position, width, height)
	{
		fingerHeldCursor = NULL;
		menu = NULL;
		scale = INITIAL_SCALE;

		setTransformToIdentity();
		initInterfaceElements();
	}

	Canvas::~Canvas(void)
	{
		for(unsigned int i = 0; i < fingers.size(); i++)
		{
			if(fingers[i] != NULL) delete fingers[i];
			fingers[i] = NULL;
		}
		fingers.clear();
	}

	/* PUBLIC MEMBER FUNCTIONS */
	bool Canvas::contains(const FingerEventArgs &e) const
	{
		Point2f p =	e.getPosition();

		assert(modelview != NULL);
		modelview->unTranslatePoint(p);

		return p.getX() >= position.getX() &&
			p.getX() <= (position.getX() + (width * modelview->getXScale())) &&
			p.getY() >= position.getY() &&
			p.getY() <= (position.getY() + (height * modelview->getYScale()));
	}

	void Canvas::fingerAdded(const FingerEventArgs &e)
	{
		Point2f p = e.getPosition();

		assert(modelview != NULL);
		modelview->unTranslatePoint(p);
		modelview->unScalePoint(p);

		Finger *f = new Finger(e.getId(), p, e.getPosition());
		fingers[e.getId()] = f;
	}

	void Canvas::fingerUpdated(const FingerEventArgs &e)
	{
		Point2f p =	e.getPosition();

		assert(modelview != NULL);
		modelview->unTranslatePoint(p);
		modelview->unScalePoint(p);

		// drag if we are tracking one finger, and this is it
		if(fingers.size() == 1 && (fingers.count(e.getId()) > 0))
		{	
			processSingleFinger(e.getId(), e.getX(), e.getY());

			Finger *f = fingers[e.getId()];

			assert(f != NULL);
			f->setLast(e.getPosition());
			f->setPosition(p);
		}
		else if(fingers.size() == Canvas::MAX_FINGERS && (fingers.count(e.getId()) > 0))
		{
			processDoubleFinger(e.getId(), e.getX(), e.getY());

			Finger *f = fingers[e.getId()];
			
			assert(f != NULL);
			f->setLast(e.getPosition());
			f->setPosition(p);
		}

		if(fingerHeldCursor != NULL)
		{
			assert(fingerHeldCursor != NULL);

			float xd = fabs(p.getX() - fingerHeldCursor->getPosition().getX()); 
			float yd = fabs(p.getY() - fingerHeldCursor->getPosition().getY()); 
			float dist = sqrt((xd * xd) + (yd * yd));

			if(dist > (width * HELD_THRESHOLD))
			{
				fingerHeldCursor->setEnabled(false);
			}
		}
	}

	void Canvas::fingerRemoved(const FingerEventArgs &e)
	{
		if(fingers.size() > 0 && fingers.count(e.getId()) > 0)
		{
			if(fingers[e.getId()] != NULL) delete fingers[e.getId()];
			fingers[e.getId()] = NULL;

			fingers.erase(e.getId());
		}

		assert(fingerHeldCursor != NULL);
		fingerHeldCursor->setEnabled(false);
	}

	RadarColumn *Canvas::getRadar(void)
	{
		return radar;
	}

	void Canvas::idle(void)
	{

	}

	void Canvas::render(void)
	{
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();

		glMultMatrixf(canvasTransform);
		saveTransform();

		glLineWidth(1.0f);
		if(dirty)
		{
			glNewList(displayList, GL_COMPILE);

			renderBackgroundMask();
				
			float xIncrement = InterfaceManager::getBeatLength() * 4;
			for(int i = 0; i <= (width / xIncrement); i++)
			{
				float lx = xIncrement * i;
				float alphaModifier =  lx / width;

				glColor4f(1.0f, 1.0f, 1.0f, OPACITY * alphaModifier);
				glBegin(GL_LINE_STRIP);
					glVertex3f(lx, 0.0f, 0.0f);
					glVertex3f(lx, height, 0.0f);
				glEnd();
			}
			
			float yIncrement = InterfaceManager::getTrackHeight() + BORDER_DIM;
			int max = (height / yIncrement);
			for(int i = 0; i < max; i++)
			{
				float ly = yIncrement * i;
			
				glBegin(GL_LINE_STRIP);
					glColor4f(1.0f, 1.0f, 1.0f, 0.0f);
					glVertex3f(0.0f, ly, 0.0f);

					glColor4f(1.0f, 1.0f, 1.0f, OPACITY);
					glVertex3f(width, ly, 0.0f);
				glEnd();
			}

			glBegin(GL_LINE_STRIP);
				glColor4f(1.0f, 1.0f, 1.0f, OPACITY);
				glVertex3f(0.0f, height, 0.0f);

				glColor4f(1.0f, 1.0f, 1.0f, 0.0f);
				glVertex3f(width / 2.0f, height, 0.0f);

				glColor4f(1.0f, 1.0f, 1.0f, OPACITY);
				glVertex3f(width, height, 0.0f);
			glEnd();
			
			glEndList();
			dirty = false;
		}
		glCallList(displayList);
		
		renderInterfaceElements();

		glPopMatrix();
	
		assert(menu != NULL);
		if(menu->isEnabled())
		{
			menu->render();
		}
	}

	void Canvas::renderVisuals(bool mainScreen)
	{
		WavesGlobalState *wgs = WavesGlobalState::getInstance();

		assert(wgs != NULL);

		for(map<int, InterfaceElement*>::iterator it = interfaceElements.begin(); it != interfaceElements.end(); it++)
		{
			InterfaceElement* element = (*it).second;
			VisualWavesControl *v = dynamic_cast<VisualWavesControl*>(element);
			
			if(v != NULL) 
			{
				assert(v != NULL);
				v->updateParameterValues();

				if(v->isOnRadar() && (v->isForOutput() || !mainScreen))
				{
					v->renderVisual();
				}
			}
		}

	}

	void Canvas::save(void)
	{
		// TODO: possibly save something about the whole canvas, i.e. which visuals?

		for(map<int, InterfaceElement *>::iterator it = interfaceElements.begin(); it != interfaceElements.end(); it++)
		{
			InterfaceElement* element = (*it).second;
			VisualWavesControl *v = dynamic_cast<VisualWavesControl*>(element);
			if(v != NULL) 
			{
				assert(v != NULL);
				v->save();
			}
		}
	}

	void Canvas::update(void)
	{
		checkForMenuGesture();
		constrainTranslate();

		assert(menu != NULL);
		menu->update();
		updateInterfaceElements();
	}

	/* PRIVATE MEMBER FUNCTIONS */
	void Canvas::checkForMenuGesture(void)
	{
		Point2f heldPos;
		
		assert(menu != NULL);
		if(!menu->isEnabled() && !fingerHeldCursor->isEnabled() && fingers.size() == 1 && GestureRecogniser::stationaryFinger(fingers, heldPos, MENU_DELAY_MS))
		{
			assert(modelview != NULL);
			modelview->unTranslatePoint(heldPos);
			modelview->unScalePoint(heldPos);

			assert(fingerHeldCursor != NULL);
			fingerHeldCursor->setPosition(heldPos);
			fingerHeldCursor->reset();
			fingerHeldCursor->setEnabled(true);
		}
	}

	void Canvas::constrainTranslate(void)
	{
		if(transformSaved)
		{
			float xt = 0.0f;
			float yt = 0.0f;
			
			assert(modelview != NULL);
			float x = modelview->getXTranslation();
			float y = modelview->getYTranslation();
			float xScale = modelview->getXScale();
			float yScale = modelview->getYScale();

			if(x > (OUTER_REGION / 4.0f))
			{
				xt = (OUTER_REGION / 4.0f) - x;
			}
			else if((x + (width * xScale)) < (InterfaceManager::WORLD_WIDTH - OUTER_REGION))
			{
				xt = InterfaceManager::WORLD_WIDTH - ((x + (width * xScale)) + OUTER_REGION);
			}
		
			if(y > OUTER_REGION)
			{
				yt = OUTER_REGION - y;
			}
			else if(y + (height * yScale) < OUTER_REGION)
			{
				yt = OUTER_REGION - (y + (height * yScale));
			}

			translateCanvas(xt, yt);
		}
	}

	void Canvas::control_Closed(VisualWavesControl *control)
	{
		WavesGlobalState *wgs = WavesGlobalState::getInstance();

		assert(wgs != NULL);
		const std::map<unsigned int, VisualWavesControl *> &savedControls = wgs->getSavedControls();

		assert(control != NULL);
		unsigned int controlId = control->getId();
		bool toDelete = (savedControls.count(controlId) == 0);

		if(toDelete)
		{
			// TODO : are we removing from the radar correctly here?
			assert(radar != NULL);
			radar->removeControl(control); // I added this assuming that we arent removing from the radar properly, could be a bug

			deleteInterfaceElement(control);
		}
		else
		{
			// 1) Disable the control
			assert(control != NULL);
			control->setEnabled(false);

			// 2) detach it from the radar
			assert(radar != NULL);
			radar->removeControl(control);

			// 3) let the menu know that it has been detached
			assert(menu != NULL);
			menu->setGrayed(control, false);
		}
	}

	void Canvas::control_FingerLifted(WavesControl *control)
	{
		assert(radar != NULL);
		assert(control != NULL);

		radar->sortSegments();
		float cx = control->getPosition().getX();
		float cy = control->getPosition().getY();
	}
	
	void Canvas::control_HeightChanged(void)
	{
		assert(radar != NULL);

		radar->sortSegments();
	}

	void Canvas::control_Moved(WavesControl* control)
	{
		assert(radar != NULL);

		radar->updateSegments(control);
	}

	void Canvas::control_Saved(VisualWavesControl *control)
	{
		//menu->setGrayed(control, true);
	}

	void Canvas::fingerHeldCursor_FingerHeld(FingerHeldCursor *fingerHeld)
	{
		assert(fingerHeldCursor != NULL);
		assert(modelview != NULL);
		assert(menu != NULL);

		Point2f heldPos = fingerHeldCursor->getPosition();

		heldPos.scaleX(modelview->getXScale());
		heldPos.scaleY(modelview->getYScale());
		heldPos.translateX(modelview->getXTranslation());
		heldPos.translateY(modelview->getYTranslation());

		menu->setPosition(heldPos);
		menu->setEnabled(true);

		fingerHeldCursor->setEnabled(false);
	}

	void Canvas::initInterfaceElements(void)
	{
		float radarWidth = (InterfaceManager::getBeatLength() * InterfaceManager::BEATS) / M_PI;
		radar = new RadarColumn(Point2f(radarWidth / 2.0f, 0.0f), radarWidth, height);
		registerInterfaceElement(radar);

		menu = new PopupMenu(Point2f(0.0f, 0.0f),  width * MENU_DIM, height * (MENU_DIM / 1.33f));
		menu->setEnabled(false);
		menu->setExitedCallback(MakeDelegate(this, &Canvas::popupMenu_Exited));
		menu->setSavedControlSelectedCallback(MakeDelegate(this, &Canvas::popupMenu_SavedControlSelected));
		menu->setVisualSelectedCallback(MakeDelegate(this, &Canvas::popupMenu_VisualSelected));
		registerEventHandler(menu);
		
		//registerInterfaceElement(menu);

		fingerHeldCursor = new FingerHeldCursor(Point2f(0.0f, 0.0f), HELD_CURSOR_SIZE, HELD_CURSOR_PERIOD);
		fingerHeldCursor->setEnabled(false);
		fingerHeldCursor->setFingerHeldCallback(MakeDelegate(this, &Canvas::fingerHeldCursor_FingerHeld));
		registerInterfaceElement(fingerHeldCursor);
	}

	void Canvas::popupMenu_Exited(void)
	{
		assert(menu != NULL);

		menu->setEnabled(false);
	}

	void Canvas::popupMenu_SavedControlSelected(VisualWavesControl *control)
	{
		assert(control != NULL);
		assert(menu != NULL);
		assert(modelview != NULL);

		Point2f p = menu->getPosition();
		modelview->unTranslatePoint(p);
		modelview->unScalePoint(p);

		control->setFingerLiftedCallback(MakeDelegate(this, &Canvas::control_FingerLifted));	
		control->setMovedCallback(MakeDelegate(this, &Canvas::control_Moved));
		control->setHeightChangedCallback(MakeDelegate(this, &Canvas::control_HeightChanged));
		control->setClosedCallback(MakeDelegate(this, &Canvas::control_Closed));
		control->setPosition(p);
		control->setEnabled(true);

		registerInterfaceElement(control);	

		menu->setEnabled(false);
	}
	
	void Canvas::popupMenu_VisualSelected(Visual* visual)
	{
		assert(menu != NULL);
		assert(modelview != NULL);
		assert(visual != NULL);

		Point2f p = menu->getPosition();
		modelview->unTranslatePoint(p);
		modelview->unScalePoint(p);

		std::vector<Track *> tracks;
		VisualWavesControl *v = new VisualWavesControl(p, InterfaceManager::getBeatLength() * 4.0f, visual, tracks);
		v->setFingerLiftedCallback(MakeDelegate(this, &Canvas::control_FingerLifted));	
		v->setMovedCallback(MakeDelegate(this, &Canvas::control_Moved));
		v->setHeightChangedCallback(MakeDelegate(this, &Canvas::control_HeightChanged));
		v->setClosedCallback(MakeDelegate(this, &Canvas::control_Closed));
		v->setSavedCallback(MakeDelegate(this, &Canvas::control_Saved));

		registerInterfaceElement(v);
		menu->setEnabled(false);
	}

	void Canvas::renderInterfaceElements(void)
	{
		assert(radar != NULL);

		radar->render();

		for(std::map<int, InterfaceElement*>::iterator it = interfaceElements.begin(); it != interfaceElements.end(); it++)
		{
			InterfaceElement *e = (*it).second;
			assert(e != NULL);

			// check its NOT a radar column
			RadarColumn *c = dynamic_cast<RadarColumn *>(e);
			if(c == NULL && e->isEnabled())
			{
				e->render();
			}
		}
	}

	void Canvas::processDoubleFinger(int actuatorId, float dx, float dy)
	{
		Finger *actuator = fingers[actuatorId];
		Finger *anchor = NULL;
		assert(actuator != NULL);

		// get the other finger
		for(map<int, Finger *>::iterator it = fingers.begin(); it != fingers.end(); it++)
		{
			int key = (*it).first;
			Finger *value = (*it).second;
			assert(value != NULL);

			if(key != actuatorId) 
			{
				anchor = value;
			}
		}

		assert(anchor != NULL);
		Vector2f v0(actuator->getLast().getX() - anchor->getLast().getX(), actuator->getLast().getY() - anchor->getLast().getY());
		Vector2f v1(dx - anchor->getLast().getX(), dy - anchor->getLast().getY());

		// calculate scale as |v2| / |v1|
		float scaleFactor = v1.getMagnitude() / v0.getMagnitude();

		scaleCanvas(scaleFactor, anchor->getLast());
	}

	void Canvas::processSingleFinger(int anchorId, float dx, float dy)
	{
		Finger *f = fingers[anchorId];
		assert(f != NULL);

		dx = dx - f->getLast().getX();
		dy = dy - f->getLast().getY();
		translateCanvas(dx, dy);
	}

	void Canvas::renderBackgroundMask(void)
	{
		glPushAttrib(GL_CURRENT_BIT);

		glColor4f(0.0f, 0.0f, 0.0f, 0.0f);
		glBegin(GL_POLYGON);
			glVertex3f(0.0f, 0.0f, 0.0f);
			glVertex3f(width, 0.0f, 0.0f);
			glVertex3f(width, height, 0.0f);
			glVertex3f(0.0f, height, 0.0f);
		glEnd();

		glPopAttrib();
	}

	void Canvas::scaleCanvas(float s, Point2f centre)
	{
		if((scale * s >= INITIAL_SCALE) && (scale * s <= MAX_SCALE))
		{
			glMatrixMode(GL_MODELVIEW);
			glPushMatrix();
			glLoadIdentity();
			glTranslatef(centre.getX(), centre.getY(), 0.0f);
			glScalef(s, s, 1.0f);
			glTranslatef(-centre.getX(), -centre.getY(), 0.0f);
			glMultMatrixf(canvasTransform);
			glGetFloatv(GL_MODELVIEW_MATRIX, canvasTransform);
			glPopMatrix();

			scale = scale * s;
		}
	}

	void Canvas::setTransformToIdentity(void)
	{
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();
		glGetFloatv(GL_MODELVIEW_MATRIX, canvasTransform);
		glPopMatrix();
	}

	void Canvas::translateCanvas(float tx, float ty)
	{
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();
		glTranslatef(tx, ty, 0.0f);
		glMultMatrixf(canvasTransform);
		glGetFloatv(GL_MODELVIEW_MATRIX, canvasTransform);
		glPopMatrix();
	}

	
	void Canvas::wavesControl_Dragged(WavesControl *control, const Vector2f &v)
	{
		assert(control != NULL);

		control->getPosition().translate(v);
	}
}