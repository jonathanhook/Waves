/**
 * Class:	Playhead
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */
#include "Playhead.h"

namespace Waves
{
	/* CONSTRUCTORS */
	Playhead::Playhead(void)
	{
		held = false;
		playheadPosition = 0;
	}

	Playhead::~Playhead(void)
	{
	}

	/* PUBLIC MEMBER FUNCTIONS */
	float Playhead::getPosition(void) const
	{
		return playheadPosition;
	}

	void Playhead::drag(float d)
	{
		float newPos = playheadPosition + d;

		if(newPos > 1.0f) newPos = newPos - 1.0f;
		else if(newPos < 0.0f) newPos = 1.0f + newPos;

		playheadPosition = newPos;
	}

	void Playhead::hold(void)
	{
		held = true;
	}
	
	bool Playhead::isHeld(void) const
	{
		return held;
	}

	void Playhead::release(void)
	{
		held = false;
	}
	
	void Playhead::setPlayheadPosition(float playheadPosition)
	{
		this->playheadPosition = playheadPosition;
	}
}
