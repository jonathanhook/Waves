/**
 * Class:	RadarSegment
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */
#include "InterfaceManager.h"
#include "JDHUtility/Interpolator.h"
#include "JDHUtility/Point2f.h"
#include "RadarSegment.h"
#include "WavesControl.h"

namespace Waves
{
	/* STATIC VARS */
	float RadarSegment::TEXTURE_OPACITY = 1.0f;

	/* CONSTRUCTORS */
	RadarSegment::RadarSegment(const Point2f &position, float width, float height, float vanishingY, WavesControl *control) :
		RadarElement(position, width, height, vanishingY)
	{
		this->control = control;
		renderOpacity = TEXTURE_OPACITY;

		assert(control != NULL);
		control->setHeightChangedCallback(MakeDelegate(this, &RadarSegment::control_HeightChanged));
	}

	RadarSegment::~RadarSegment(void)
	{

	}

	/* PUBLIC MEMBER FUNCTIONS */
	bool RadarSegment::contains(const FingerEventArgs &e) const
	{
		return false;
	}

	WavesControl *RadarSegment::getControl(void)
	{
		return control;
	}

	void RadarSegment::render(void)
	{
		assert(control != NULL);

		unsigned int texture = control->renderRadarSegmentTexture();

		if(texture != -1)
		{
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, texture);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		}

		RadarElement::render();
	}

	/* PRIVATE MEMBER FUNCTIONS */
	void RadarSegment::control_HeightChanged(void)
	{
		assert(control != NULL);

		height = control->getHeight();
	}

	void RadarSegment::detach(void)
	{
		assert(control != NULL);

		control->getPosition().setY(position.getY());
		control->setOnRadar(false);
	
		if(detached != NULL) 
		{
			detached(this);
		}
	}

	float RadarSegment::getOffsetY(float yVal, float zVal) const
	{
		assert(control != NULL);

		// make the actual size at the centre (z)
		zVal -= 0.5f;
		yVal += (control->getPosition().getY() - (InterfaceManager::WORLD_HEIGHT / 2.0f));

		float y0 = yVal + position.getY();
		float y1 = vanishingY;
		float mu = zVal / InterfaceManager::VANISHING_POINT_Z;
		float returnVal = Interpolator::linearInterpolate(y0, y1, mu) - position.getY();

		return returnVal - (control->getPosition().getY() - (InterfaceManager::WORLD_HEIGHT / 2.0f));
	}
}