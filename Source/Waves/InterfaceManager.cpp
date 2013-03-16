/**
 * Class:	InterfaceManager
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */
#include <assert.h>
#include "Canvas.h"
#include "Finger.h"
#include "JDHUtility/FPSMonitor.h"
#include "JDHUtility/GLContext.h"
#include "JDHUtility/GLFontRenderer.h"
#include "JDHUtility/GlutInclude.h"
#include "JDHUtility/Point2f.h"
#include "JDHUtility/Vector2f.h"
#include "InterfaceElement.h"
#include "InterfaceManager.h"
#include "MultiTouchEvents/MultiTouchEventHandler.h"
#include "WavesGlobalState.h"

using namespace JDHUtility;
using namespace MultiTouchEvents;

namespace Waves
{	
	/* PUBLIC CONSTANTS */
	const unsigned int	InterfaceManager::BEATS				= 64;
	const float			InterfaceManager::CAMERA_Z			= 1.0f;
	const float			InterfaceManager::CAMERA_FOV		= 45.0f;
	const unsigned int	InterfaceManager::TRACK_DIV			= 22;
	const unsigned int	InterfaceManager::VALUES_PER_BEAT	= 4;
	const float			InterfaceManager::VISUAL_Z			= -0.5f;
	const float			InterfaceManager::WORLD_WIDTH		= 1.0f;
	const float			InterfaceManager::WORLD_HEIGHT		= 1.0f;
	const float			InterfaceManager::VANISHING_POINT_Z = 10.0f;

	/* STATIC FUNCTONS */
	float InterfaceManager::getBeatLength(void)
	{
		return WORLD_WIDTH / (float)BEATS;
	}
	
	float InterfaceManager::getTrackHeight(void)
	{
		return WORLD_HEIGHT / (float)TRACK_DIV;
	}

	/* CONSTRUCTORS */
	InterfaceManager::InterfaceManager(bool dispatchEvents) :
		MultiTouchEventManager(dispatchEvents)
	{
		interfaceRoot	= new Canvas(Point2f(0.0f, 0.0f), WORLD_WIDTH, WORLD_HEIGHT);

		registerEventHandler(interfaceRoot);
	}

	InterfaceManager::~InterfaceManager(void)
	{
		if(interfaceRoot != NULL) delete interfaceRoot;
		interfaceRoot = NULL;
	}

	/* PUBLIC MEMBER FUNCTIONS */
	void InterfaceManager::renderInterface()
	{
		assert(interfaceRoot != NULL);

		interfaceRoot->render();

		for(std::map<unsigned int, Finger *>::iterator it = fingers.begin(); it != fingers.end(); it++)
		{
			Finger *f = (*it).second;
			assert(f != NULL);

			f->render();
		}

		reportFPS();
	}

	void InterfaceManager::renderVisuals(bool mainScreen)
	{
		Canvas *c = (Canvas *)interfaceRoot;
		assert(c != NULL);

		c->renderVisuals(mainScreen);
	}

	void InterfaceManager::save(void)
	{
		// TODO : the global save function, for now its just a 
		// hook to allow for save testing

		Canvas *c = (Canvas *)interfaceRoot;
		assert(c != NULL);

		c->save();
	}

	void InterfaceManager::update(void)
	{
		assert(interfaceRoot != NULL);
		
		dispatchAllEvents();
		interfaceRoot->update();

		WavesGlobalState *wgs = WavesGlobalState::getInstance();
		assert(wgs != NULL);

		wgs->updatePlayheadPosition();
	}

	/* PRIVATE MEMBER FUNCTIONS */
	void InterfaceManager::peek(const FingerEventArgs &e)
	{
		unsigned int eId = e.getId();
		switch(e.getType())
		{
			case FingerEventArgs::FINGER_ADDED: 
				fingers[eId] = new Finger(e.getId(), e.getPosition(), e.getPosition());
				break;
			case FingerEventArgs::FINGER_UPDATED: 
				if(fingers.count(eId) > 0)
				{	
					Finger *f = fingers[eId];
					assert(f != NULL);

					f->setLast(e.getPosition());
					f->setPosition(e.getPosition());
				}
				break;
			case FingerEventArgs::FINGER_REMOVED: 
				if(fingers[eId] != NULL) 
				{
					delete fingers[e.getId()];
					fingers[eId] = NULL;
				}
				
				fingers.erase(e.getId());
				
				break;
		}
	}

	void InterfaceManager::reportFPS(void)
	{
		WavesGlobalState *wgs = WavesGlobalState::getInstance();
		assert(wgs != NULL);

		FPSMonitor &fpsMonitor = wgs->getFPSMonitor();
		fpsMonitor.markRender(glutGet(GLUT_ELAPSED_TIME));
		float fps = fpsMonitor.getFPS();
		float bpm = wgs->getBpm();

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();

		glPushAttrib(GL_CURRENT_BIT | GL_LINE_WIDTH);
		glLineWidth(1.0f);
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

		GLContext *currContext = GLContext::getCurrentContext();
		assert(currContext != NULL);

		char str[16];
		sprintf(str, "BPM: %3.1f", bpm);
		glTranslatef(0.01f, 0.01f, 0.0f);
		currContext->renderString(str, Vector2f(0.07f, 0.0125f));

		sprintf(str, "FPS: %2.1f", fps);
		glTranslatef(0.00f, 0.0175f, 0.0f);		
		currContext->renderString(str, Vector2f(0.0625f, 0.0125f));

		glPopAttrib();
		glPopMatrix();
	}
}




