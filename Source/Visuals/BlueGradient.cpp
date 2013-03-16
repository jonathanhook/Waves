/**
 * Class:	BlueGradient
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */
#include "Visuals/BlueGradient.h"

namespace WavesVisuals
{
	/* PRIVATE CONSTANTS */
	const std::string BlueGradient::IMG_PATH = "../Images/Backgrounds/BlueGradient.tga";

	/* CONSTUCTORS */
	BlueGradient::BlueGradient(void) :
		BackgroundImg(IMG_PATH, "Blue Grad")
	{
	}

	BlueGradient::~BlueGradient(void)
	{
	}

	/* PUBLIC MEMBER FUNCTIONS */
	Visual *BlueGradient::getInstance(void) const
	{
		return new BlueGradient();
	}
}

