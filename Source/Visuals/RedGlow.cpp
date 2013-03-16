/**
 * Class:	RedGlow
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */
#include "Visuals/RedGlow.h"

namespace WavesVisuals
{
	/* PRIVATE CONSTANTS */
	const std::string RedGlow::IMG_PATH = "../Images/Backgrounds/RedGlow.tga";

	/* CONSTUCTORS */
	RedGlow::RedGlow(void) :
		BackgroundImg(IMG_PATH, "Red Glow")
	{
	}

	RedGlow::~RedGlow(void)
	{
	}

	/* PUBLIC MEMBER FUNCTIONS */
	Visual *RedGlow::getInstance(void) const
	{
		return new RedGlow();
	}
}