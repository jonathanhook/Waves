/**
 * Class:	LoadGridScrollMenu
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */
#include "FastDelegate.h"
#include "JDHUtility/GlutInclude.h"
#include "LoadGridScrollMenuItem.h"
#include "VisualWavesControl.h"

using namespace fastdelegate;

namespace Waves
{
	/* CONSTRUCTORS */
	LoadGridScrollMenuItem::LoadGridScrollMenuItem(const Point2f &position, float width, float height, VisualWavesControl *control) :
		GridScrollMenuItem(position, width, height, control->getVisual())
	{
		this->control = control;
		grayed = false;

		std::string nickname = control->getNickname();
		if(nickname != "")
		{
			textLabel = nickname;
		}
	}

	LoadGridScrollMenuItem::~LoadGridScrollMenuItem(void)
	{
	}

	/* PUBLIC MEMBER FUNCTIONS */	
	void LoadGridScrollMenuItem::fingerAdded(const FingerEventArgs &e)
	{
		if(!grayed)
		{
			GridScrollMenuItem::fingerAdded(e);
		}
	}

	void LoadGridScrollMenuItem::fingerUpdated(const FingerEventArgs &e)
	{
		if(!grayed)
		{
			GridScrollMenuItem::fingerUpdated(e);
		}
	}

	void LoadGridScrollMenuItem::fingerRemoved(const FingerEventArgs &e)
	{
		Button::fingerRemoved(e);

		if(!grayed && savedControlSelected != NULL && contains(e))
		{
			savedControlSelected(control);
			grayed = true;
		}
	}

	const VisualWavesControl &LoadGridScrollMenuItem::getControl(void) const
	{
		return *control;
	}

	void LoadGridScrollMenuItem::render(void)
	{
		GridScrollMenuItem::render();

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		
		glPushAttrib(GL_CURRENT_BIT);

		glTranslatef(position.getX(), position.getY(), 0.0f);
		saveTransform();
		
		if(grayed)
		{
			glColor4f(0.0f, 0.0f, 0.0f, 0.5f);
			glBegin(GL_QUADS);
				glVertex3f(0.0f, 0.0f, 0.0f);
				glVertex3f(width, 0.0f, 0.0f);
				glVertex3f(width, height, 0.0f);
				glVertex3f(0.0f, height, 0.0f);
			glEnd();
		}
	
		glPopAttrib();
		glPopMatrix();
	}

	void LoadGridScrollMenuItem::setGrayed(bool grayed)
	{
		this->grayed = grayed;
	}

	void LoadGridScrollMenuItem::setSavedControlSelectedCallback(SavedControlSelectedCallback savedControlSelected)
	{
		this->savedControlSelected = savedControlSelected;
	}

	/* PRIVATE MEMBER FUNCTIONS */
	void LoadGridScrollMenuItem::control_Closed(VisualWavesControl *control)
	{
		grayed = false;
	}
}
