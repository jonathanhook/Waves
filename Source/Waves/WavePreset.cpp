/**
 * Class:	WavePreset
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */
#include "JDHUtility/Point2f.h"
#include "WavePreset.h"

using namespace JDHUtility;

namespace Waves
{
	/* PUBLIC CONSTANTS */
	const Point2f WavePreset::BAND_PASS[3] = {Point2f(0.2f, 1.0f), Point2f(0.5f, 0.0f), Point2f(0.8f, 1.0f)};
	const Point2f WavePreset::HIGH_PASS[3] = {Point2f(0.999f, 0.0f), Point2f(0.85f, 0.0f), Point2f(0.8f, 1.0f)};
	const Point2f WavePreset::LOW_PASS[3] = {Point2f(0.001f, 0.0f), Point2f(0.15f, 0.0f), Point2f(0.2f, 1.0f)};
	const Point2f WavePreset::SINE_WAVE[2] = {Point2f(0.001f, 1.0f), Point2f(0.5f, 0.0f)};
	const Point2f WavePreset::SQUARE_WAVE[4] = {Point2f(0.001f, 0.0f), Point2f(0.499f, 0.0f), Point2f(0.5f, 1.0f), Point2f(0.999f, 1.0f)};
	const Point2f WavePreset::SAWTOOTH_WAVE[3] = {Point2f(0.001f, 0.999f), Point2f(0.995f, 0.001f), Point2f(0.999f, 0.999f)};
}
