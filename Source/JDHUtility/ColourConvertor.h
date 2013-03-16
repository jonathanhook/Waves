/**
 * Class:	ColourConvertor
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */
#ifndef ColourConvertor_H
#define ColourConvertor_H

#include "Colour3f.h"
#include "Vector3f.h"

namespace JDHUtility
{
	class ColourConvertor
	{
	public:
		static Colour3f hslToRgb(const Vector3f &rgb);
		static Vector3f rgbToHsl(const Colour3f &rgb);

	private:
		static float clamp(float f);
		static void ColourConvertor::findMaxMin(float r, float g, float b, float &max, float &min);
		static float hslClamp(float f);
	};
}
#endif
