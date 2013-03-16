/**
 * Class:	WavesControl
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */
#include "AddToggleButton.h"
#include "DragButton.h"
#include "ExitButton.h"
#include "FastDelegate.h"
#include "JDHUtility/GLContext.h"
#include "InterfaceManager.h"
#include "Visuals/Parameter.h"
#include "Playhead.h"
#include "RadarSegment.h"
#include "TextScrollMenu.h"
#include "Track.h"
#include "WavesControl.h"
#include "WavesGlobalState.h"

using namespace fastdelegate;

namespace Waves
{
	/* PROTECTED CONSTANTS */
	const float WavesControl::SCROLL_MENU_HEIGHT = 0.05f;
	const float WavesControl::SCROLL_MENU_WIDTH = 0.05f;

	/* PUBLIC STATIC FUNCTIONS */
	bool WavesControl::compare(WavesControl* lhs, WavesControl* rhs)
	{
		assert(lhs != NULL);
		assert(rhs != NULL);

		return lhs->getPosition().getY() < rhs->getPosition().getY();
	}

	unsigned int WavesControl::depthBuffer;
	unsigned int WavesControl::frameBufferObject;
	unsigned int WavesControl::texture;
	void WavesControl::setupFbo(void)
	{
		static bool setup = false;

		if(!setup)
		{
			glGenFramebuffersEXT(1, &frameBufferObject);
			glGenRenderbuffersEXT(1, &depthBuffer);

			glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, frameBufferObject);
			glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, depthBuffer);

			glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT, TEXTURE_WIDTH, TEXTURE_HEIGHT);
			glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, depthBuffer);

			glGenTextures(1, &texture);
			glBindTexture(GL_TEXTURE_2D, texture);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, TEXTURE_WIDTH, TEXTURE_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
			glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, texture, 0);

			glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

			setup = true;
		}
	}

	/* CONSTRUCTORS */
	WavesControl::WavesControl(const Point2f &position, float width, std::string title, std::vector<Track *> tracks) :
		InterfaceElement(position, width, 0.0f)
	{
		this->title = title;
		this->tracks = tracks;

		fingerLifted = NULL;
		heightChanged = NULL;
		onRadar = false;
		moved = NULL;

		// fbo
		setupFbo();

		//initInterfaceElements();
	}

	WavesControl::~WavesControl(void)
	{
	}

	/* PUBLIC MEMBER FUNCTIONS */
	void WavesControl::addTrack(Parameter *parameter)
	{
		// calculate dimensions
		float trackHeight = InterfaceManager::getTrackHeight();
		float xPos = (BAR_DIM * 2.0f) + BORDER_DIM;
		float yPos = (trackHeight * (float)(tracks.size() + 1)) + BORDER_DIM;
		float borderOffsetY = BORDER_DIM * tracks.size();

		// add the track
		Track *t = new Track(Point2f(xPos, yPos + borderOffsetY), width, trackHeight, parameter);
		t->setExitedCallback(MakeDelegate(this, &WavesControl::track_Exited));
		tracks.push_back(t);
		
		registerInterfaceElement(t);

		updateHeight();

		if(heightChanged != NULL) heightChanged();
	}

	bool WavesControl::contains(const FingerEventArgs &e) const
	{
		return false;
	}

	void WavesControl::fingerAdded(const FingerEventArgs &e)
	{

	}

	void WavesControl::fingerUpdated(const FingerEventArgs &e)
	{

	}

	void WavesControl::fingerRemoved(const FingerEventArgs &e)
	{

	}

	RadarSegment &WavesControl::getSegment(void)
	{
		return *segment;
	}

	bool WavesControl::isOnRadar(void) const
	{
		return onRadar;
	}
	
	void WavesControl::render(void)
	{
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();

		glTranslatef(position.getX(), position.getY(), 0.0f);
		saveTransform();

		renderInterfaceElements();

		glPopMatrix();
	}

	unsigned int WavesControl::renderRadarSegmentTexture(void)
	{
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, frameBufferObject);
			
		glPushAttrib(GL_ALL_ATTRIB_BITS);
		glViewport(0, 0, TEXTURE_WIDTH, TEXTURE_HEIGHT);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();

		float textHeight = InterfaceManager::getTrackHeight();
		float textWidth = (float)title.size() * textHeight;

		glColor4f(1.0f, 1.0f, 1.0f, 0.75f);	

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glTranslatef(0.0f, textHeight * 0.05f, 0.0f);

		unsigned int repeats = (int)(1.0f / textWidth);
		float repeatDiv = 1.0f / (float)repeats;
		for(unsigned int i = 0; i < repeats; i++)
		{
			GLContext *currentContext = GLContext::getCurrentContext();
			assert(currentContext != NULL);
			currentContext->renderString(title, Vector2f(textWidth, textHeight), 0.15f);	
			glTranslatef(repeatDiv, 0.0f, 0.0f);
		}

		glPopMatrix();

		for(unsigned int i = 0; i < tracks.size(); i++)
		{
			Track *tri =  tracks[i];
			assert(tri != NULL);

			glTranslatef(0.0f, tri->getHeight() + BORDER_DIM, 0.0f);
			tri->renderSpline();
		}
			
		glPopMatrix();
		glPopAttrib();
		
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

		return texture;
	}

	void WavesControl::setFingerLiftedCallback(FingerLiftedCallback fingerLifted)
	{
		this->fingerLifted = fingerLifted;
	}

	void WavesControl::setMovedCallback(MovedCallback moved)
	{
		this->moved = moved;
	}

	void WavesControl::setHeightChangedCallback(HeightChangedCallback heightChanged)
	{
		this->heightChanged = heightChanged;
	}

	void WavesControl::setOnRadar(bool onRadar)
	{
		this->onRadar = onRadar;
	}

	void WavesControl::update(void)
	{
		assert(addButton != NULL);

		if(addButton->isToggled())
		{
			int elapsedTime = glutGet(GLUT_ELAPSED_TIME) - menuLastInteraction;
			if(elapsedTime > MENU_HIDE_DELAY) hideMenu();
		}

		updateInterfaceElements();
	}

	/* PROTECTED MEMBER FUNCTIONS */
	void WavesControl::hideMenu(void)
	{
		assert(menu != NULL);
		assert(addButton != NULL);

		deleteInterfaceElement(menu);
		menu = NULL; // TODO : POSSIBLE CRASH
		addButton->setToggled(false);
	}

	void WavesControl::showMenu(vector<std::string> items)
	{
		if(items.size() > 0)
		{
			for(std::vector<std::string>::iterator it = items.begin(); it != items.end(); )
			{
				std::string s = (*it);

				bool deleted = false;
				for(unsigned int j = 0; j < tracks.size(); j++)
				{
					Track *tj = tracks[j];
					assert(tj != NULL);

					if(s == tj->getParameter().getName())
					{
						it = items.erase(it);
						deleted = true;
						break;
					}
				}

				if(!deleted) it++;
			}

			menu = new TextScrollMenu(Point2f(0.0f, height + BORDER_DIM), SCROLL_MENU_HEIGHT, items);
			menu->setItemSelected(MakeDelegate(this, &WavesControl::menu_ItemSelected));
			menu->setTouchedCallback(MakeDelegate(this, &WavesControl::menu_Touched));
			registerInterfaceElement(menu);

			menuLastInteraction = glutGet(GLUT_ELAPSED_TIME);
		}
	}

	/* PROTECTED MEMBER FUNCTIONS */
	void WavesControl::initInterfaceElements(void)
	{	
		float titleWidth = BAR_DIM * 2.0f;

		// title bar
		titleBar = new DragButton(Point2f(0.0f, 0.0f), titleWidth, 0.0f);
		titleBar->setText(title);
		titleBar->setTextDirection(DragButton::VERTICAL);
		titleBar->setDraggedCallback(MakeDelegate(this, &WavesControl::titleBar_dragged));
		titleBar->setLiftedCallback(MakeDelegate(this, &WavesControl::titleBar_fingerLifted));
		registerInterfaceElement(titleBar);

		// add buttom
		addButton = new AddToggleButton(Point2f(0.0f, 0.0f), titleWidth, titleWidth);
		addButton->setToggleChangedCallback(MakeDelegate(this, &WavesControl::addToggleButton_ToggleChanged));
		registerInterfaceElement(addButton);

		// add segment
		segment = new RadarSegment(Point2f(0.0f, 0.0f), width, height, height / 2.0f, this);
		segment->setEnabled(false);
		registerInterfaceElement(segment);

		// add loaded tracks
		for(unsigned int i = 0; i < tracks.size(); i++)
		{
			Track *track = tracks[i];
			assert(track != NULL);

			track->setExitedCallback(MakeDelegate(this, &WavesControl::track_Exited));
			registerInterfaceElement(track);

			if(heightChanged != NULL) 
			{
				heightChanged();
			}
		}

		updateHeight();
	}

	/* PRIVATE MEMBER FUNCTIONS */
	void WavesControl::menu_Touched(void)
	{
		menuLastInteraction = glutGet(GLUT_ELAPSED_TIME);
	}

	void WavesControl::removeTrack(Track *track)
	{
		assert(track != NULL);

		float y = track->getPosition().getY();
		for(std::vector<Track *>::iterator it = tracks.begin(); it != tracks.end(); )
		{
			Track *t1 = (*it);
			assert(t1 != NULL);

			float t1Y = t1->getPosition().getY();

			if(t1 != track && t1Y > y)
			{
				t1->getPosition().translateY(-(track->getHeight() + BORDER_DIM));
			}

			if(t1 == track)
			{
				it = tracks.erase(it);
			}
			else it++;

		}

		updateHeight();
		deleteInterfaceElement(track);
	}

	void WavesControl::titleBar_dragged(const Vector2f &v)
	{
		assert(modelview != NULL);

		Vector2f v1 = v;
		modelview->unScalePoint(v1);
		position.translate(v1);

		if(moved != NULL)
		{
			moved(this);
		}
	}

	void WavesControl::titleBar_fingerLifted(void)
	{
		if(fingerLifted != NULL) fingerLifted(this);
	}

	void WavesControl::track_Exited(Track *track)
	{
		removeTrack(track);
	}

	void WavesControl::updateHeight(void)
	{
		assert(titleBar != NULL);
		assert(addButton != NULL);
		assert(segment != NULL);

		float trackHeight = InterfaceManager::getTrackHeight();
		float borderOffsetY = BORDER_DIM * (float)(tracks.size() + 1);

		// update the size of the main control
		height = (trackHeight * (float)(tracks.size() + 1)) + borderOffsetY - BORDER_DIM;
		titleBar->setHeight(height - (BAR_DIM * 2.0f));
		addButton->getPosition().setY(height - (BAR_DIM * 2.0f));
		segment->setHeight(height);
	
		// mark dirty
		dirty = true;
		titleBar->setDirty(true);

		// POTENTIAL BUG
		if(addButton->isToggled() && menu != NULL)
		{
			assert(menu != NULL);
			menu->getPosition().setY(height);
		}

		if(heightChanged != NULL)
		{
			heightChanged();
		}
	}
}
