/**
 * Class:	PurpleGradient
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */
#include "Visuals/PurpleGradient.h"

namespace WavesVisuals
{
	/* PRIVATE CONSTANTS */
	const std::string PurpleGradient::IMG_PATH = "../Images/Backgrounds/PurpleGradient.tga";

	/* CONSTUCTORS */
	PurpleGradient::PurpleGradient(void) :
		BackgroundImg(IMG_PATH, "Blue Grad")
	{
	}

	PurpleGradient::~PurpleGradient(void)
	{
	}

	/* PUBLIC MEMBER FUNCTIONS */
	Visual *PurpleGradient::getInstance(void) const
	{
		return new PurpleGradient();
	}
}