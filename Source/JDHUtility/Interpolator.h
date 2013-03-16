/**
 * Class:	Interpolator
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */
#ifndef Interpolator_H
#define Interpolator_H

#include <vector>

namespace JDHUtility
{
	class Interpolator
	{
	public:
		typedef enum
		{
			LINEAR,
			CUBIC,
			HERMITE
		} Type;

		static float DEFAULT_TENSION;

		static float hermiteInterpolate(float y0,float y1, float y2, float y3, float mu, float tension, float bias);
		static float cubicInterpolate(float y0, float y1, float y2, float y3, float mu);
		static float linearInterpolate(float y0, float y1, float mu);
		static float interpolateValue(float* values, int length, int index, float mu, int type);
		static float interpolateValue(std::vector<float> values, int length, int index, float mu, int type);
		static float wrappedInterpolateValue(float* values, int length, int index, float mu, int type);
		static float wrappedInterpolateValue(std::vector<float> values, int length, int index, float mu, int type);
	};
}
#endif
