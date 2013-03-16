/**
 * Class:	GreenGlow
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */
#include "Visuals/GreenGlow.h"

namespace WavesVisuals
{
	/* PRIVATE CONSTANTS */
	const std::string GreenGlow::IMG_PATH = "../Images/Backgrounds/GreenGlow.tga";

	/* CONSTUCTORS */
	GreenGlow::GreenGlow(void) :
		BackgroundImg(IMG_PATH, "Green Glow")
	{
	}

	GreenGlow::~GreenGlow(void)
	{
	}

	/* PUBLIC MEMBER FUNCTIONS */
	Visual *GreenGlow::getInstance(void) const
	{
		return new GreenGlow();
	}
}

