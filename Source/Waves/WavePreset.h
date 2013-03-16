/**
 * Class:	WavePreset
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */
#ifndef WavePreset_H
#define WavePreset_H

#include <vector>

namespace JDHUtility
{
	class Point2f;
}

using namespace JDHUtility;

namespace Waves
{
	class WavePreset
	{
	public:
		static const Point2f BAND_PASS[3];
		static const Point2f HIGH_PASS[3];
		static const Point2f LOW_PASS[3];

		static const Point2f SINE_WAVE[2];
		static const Point2f SQUARE_WAVE[4];
		static const Point2f SAWTOOTH_WAVE[3];
	};
}
#endif

