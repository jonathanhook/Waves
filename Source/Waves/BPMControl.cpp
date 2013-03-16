/**
 * Class:	BPMControl
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */
#include "AddButton.h"
#include "BPMControl.h"
#include "Button.h"
#include "JDHUtility/GlutInclude.h"
#include "TapButton.h"
#include "TapBpm/TapBpm.h"
#include "TextLabel.h"
#include "SubtractButton.h"
#include "WavesGlobalState.h"

namespace Waves
{
	/* PRIVATE CONSTANTS */
	const float BPMControl::AS_WIDTH = 0.2f;
	const float BPMControl::BPM_INCREMENT = 0.1f;
	const float BPMControl::TAP_HEIGHT = 0.8f;
	const float BPMControl::TAP_WIDTH = 0.6f;

	/* CONSTRUCTORS */
	BPMControl::BPMControl(const Point2f position, float width, float height) :
		InterfaceElement(position, width, height)
	{
		addHeld = false;
		last = glutGet(GLUT_ELAPSED_TIME);
		lastTap = glutGet(GLUT_ELAPSED_TIME);
		subtractHeld = false;
		tapBpm = new TapBpm();

		initInterfaceElements();
	}

	BPMControl::~BPMControl(void)
	{
	}

	/* PUBLIC MEMBER FUNCTIONS */
	bool BPMControl::contains(const FingerEventArgs &e) const
	{
		return false;
	}

	void BPMControl::fingerAdded(const FingerEventArgs &e)
	{
		
	}
	
	void BPMControl::fingerUpdated(const FingerEventArgs &e)
	{
		
	}
	
	void BPMControl::fingerRemoved(const FingerEventArgs &e)
	{

	}

	void BPMControl::render(void)
	{
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();

		glTranslatef(position.getX(), position.getY(), 0.0f);
		saveTransform();

		if(displayList)
		{
			glNewList(displayList, GL_COMPILE);
	
			glPushAttrib(GL_LINE_BIT | GL_CURRENT_BIT);
			glLineWidth(1.0f);
			glColor4f(1.0f, 1.0f, 1.0f, LINE_OPACITY);

			glBegin(GL_LINE_LOOP);
				glVertex3f(0.0f, 0.0f, 0.0f);
				glVertex3f(width, 0.0f, 0.0f);
				glVertex3f(width, height, 0.0f);
				glVertex3f(0.0f, height, 0.0f);
			glEnd();

			glPopAttrib();

			glEndList();

			dirty = false;
		}
		glCallList(displayList);

		renderInterfaceElements();

		glPopMatrix();	
	}

	void BPMControl::update(void)
	{
		int now = glutGet(GLUT_ELAPSED_TIME);

		if(now - last > UPDATE_RATE)
		{
			if(addHeld)
			{
				int doubles = (now - addHeldTime) / DOUBLE_RATE;
				float increment = BPM_INCREMENT * (float)doubles;
				alterBpm(increment);
			}

			if(subtractHeld)
			{
				int doubles = (now - subtractHeldTime) / DOUBLE_RATE;
				float increment = BPM_INCREMENT * (float)doubles;
				alterBpm(-increment);
			}

			last = now;
		}
	}

	/* PRIVATE MEMBER FUNCTIONS */
	void BPMControl::addButton_Lifted(void)
	{
		addHeld = false;
	}

	void BPMControl::addButton_Pressed(void)
	{
		alterBpm(BPM_INCREMENT);
		addHeld = true;
		addHeldTime = glutGet(GLUT_ELAPSED_TIME);
	}

	void BPMControl::applyButton_Clicked(void)
	{
		WavesGlobalState *wgs = WavesGlobalState::getInstance();
		
		assert(wgs != NULL);
		wgs->setBpm(tapBpm->getBpm());
		alterBpm(0.0f);
	}

	void BPMControl::alterBpm(float bpm)
	{
		WavesGlobalState *wgs = WavesGlobalState::getInstance();
		
		float newBpm = wgs->getBpm() + bpm;

		assert(wgs != NULL);
		wgs->setBpm(newBpm);

		if(newBpm > 200.0f) newBpm = 200.0f;
		else if(newBpm < 0.1f) newBpm = 0.1f;

		char buff[256];
		sprintf(buff, "%2.1f", newBpm);
		
		assert(label != NULL);
		label->setText(buff);
	}

	void BPMControl::initInterfaceElements(void)
	{
		float borderDim = BORDER_DIM * 5.0f;

		tapButton = new TapButton(Point2f(borderDim, borderDim), (width * TAP_WIDTH) - borderDim, (height * TAP_HEIGHT) - borderDim);
		tapButton->setPressedCallback(MakeDelegate(this, &BPMControl::tapButton_Pressed));
		tapButton->reset();
		registerInterfaceElement(tapButton);

		float buttonWidth = ((width * TAP_WIDTH) / 2.0f) - borderDim; 
		float buttonHeight = (height * (1.0f - TAP_HEIGHT)) - (borderDim * 2.0f);
		Button *resetButton = new Button(Point2f(borderDim, borderDim + (height * TAP_HEIGHT)), buttonWidth, buttonHeight);
		resetButton->setClickedCallback(MakeDelegate(this, &BPMControl::resetButton_Clicked));
		resetButton->setText("Reset");

		registerInterfaceElement(resetButton);

		Button *applyButton = new Button(Point2f((borderDim * 2.0f) + buttonWidth, borderDim + (height * TAP_HEIGHT)), buttonWidth, buttonHeight);
		applyButton->setText("Apply");
		applyButton->setClickedCallback(MakeDelegate(this, &BPMControl::applyButton_Clicked));
		registerInterfaceElement(applyButton);

		float asButtonWidth = (buttonHeight * 1.75f);
		float asButtonXPos = width - ((width * ((1.0f - TAP_WIDTH) / 2.0f)) + (asButtonWidth / 2.0f));
		
		AddButton *addButton = new AddButton(Point2f(asButtonXPos, borderDim * 3.0f), asButtonWidth, asButtonWidth);
		addButton->setPressedCallback(MakeDelegate(this, &BPMControl::addButton_Pressed));
		addButton->setLiftedCallback(MakeDelegate(this, &BPMControl::addButton_Lifted));
		registerInterfaceElement(addButton);

		SubtractButton *subtractButton = new SubtractButton(Point2f(asButtonXPos, height - (borderDim * 3.0f) - asButtonWidth), asButtonWidth, asButtonWidth);
		subtractButton->setPressedCallback(MakeDelegate(this, &BPMControl::subtractButton_Pressed));
		subtractButton->setLiftedCallback(MakeDelegate(this, &BPMControl::subtractButton_Lifted));
		registerInterfaceElement(subtractButton);

		float labelWidth = (1.0f - TAP_WIDTH) * width;

		WavesGlobalState *wgs = WavesGlobalState::getInstance();

		assert(wgs != NULL);
		float currentBpm = wgs->getBpm();	
		
		char buff[256];
		sprintf(buff, "%2.1f", currentBpm);

		label = new TextLabel(Point2f(width * TAP_WIDTH, (height / 2.0f) - (buttonHeight / 4.0f)), labelWidth, (buttonHeight / 1.5f), buff);
		label->setCentered(true);
		registerInterfaceElement(label);
	}

	void BPMControl::resetButton_Clicked(void)
	{
		// asser ptrs
		assert(tapBpm != NULL);
		assert(tapButton != NULL);

		tapBpm->reset();
		tapButton->reset();
	}

	void BPMControl::subtractButton_Lifted(void)
	{
		subtractHeld = false;
	}

	void BPMControl::subtractButton_Pressed(void)
	{
		alterBpm(-BPM_INCREMENT);
		subtractHeld = true;
		subtractHeldTime = glutGet(GLUT_ELAPSED_TIME);
	}

	void BPMControl::tapButton_Pressed(void)
	{
		// assert ptrs
		assert(tapBpm != NULL);
		assert(tapButton != NULL);

		int now = glutGet(GLUT_ELAPSED_TIME);

		if(now - lastTap < RESET_TIME)
		{
			tapBpm->tap();
			tapButton->setBpm(tapBpm->getBpm());
		}
		else
		{
			tapBpm->reset();
			tapButton->reset();
		}

		lastTap = now;
	}
}
