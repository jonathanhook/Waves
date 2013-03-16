/**
 * Class:	Interpolator
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */
#include <vector>
#include "JDHUtility/Interpolator.h"

namespace JDHUtility
{
	/* STATIC VARS */
	float Interpolator::DEFAULT_TENSION = 0.0f;

	/* PUBLIC STATIC FUNCTIONS */
	float Interpolator::cubicInterpolate(float y0, float y1, float y2, float y3, float mu)
	{
		mu+=2;

		float a0, a1, a2, a3, mu2;

		mu2 = mu * mu;
		a0 = y3 - y2 - y0 + y1;
		a1 = y0 - y1 - a0;
		a2 = y2 - y0;
		a3 = y1;

		return(a0 * mu * mu2 + a1 * mu2 + a2 * mu + a3);
	}

	float Interpolator::hermiteInterpolate(float y0,float y1, float y2, float y3, float mu, float tension, float bias)
	{
	   float m0,m1,mu2,mu3;
	   float a0,a1,a2,a3;

		mu2 = mu * mu;
		mu3 = mu2 * mu;
		m0  = (y1-y0)*(1+bias)*(1-tension)/2;
		m0 += (y2-y1)*(1-bias)*(1-tension)/2;
		m1  = (y2-y1)*(1+bias)*(1-tension)/2;
		m1 += (y3-y2)*(1-bias)*(1-tension)/2;
		a0 =  2*mu3 - 3*mu2 + 1;
		a1 =    mu3 - 2*mu2 + mu;
		a2 =    mu3 -   mu2;
		a3 = -2*mu3 + 3*mu2;

	   return(a0 * y1 + a1 * m0 + a2 * m1 + a3 * y2);
	}

	// values clamped at ends
	float Interpolator::interpolateValue(float* values, int length, int index, float mu, int type)
	{
		float y0, y1, y2, y3;
		y1 = values[index];

		if(index == 0)
		{
			y0 = values[index];
			y2 = values[index + 1];
			y3 = values[index + 2];
		}
		else if(index == (length - 2))
		{
			y0 = values[index - 1];
			y2 = values[index + 1];
			y3 = values[index + 1];
		}
		else if(index == (length - 1))
		{
			y0 = values[index - 1];
			y2 = values[index];
			y3 = values[index];
		}
		else
		{
			y0 = values[index - 1];
			y2 = values[index + 1];
			y3 = values[index + 2];
		}

		if(type == LINEAR)
		{
			return linearInterpolate(y1, y2, mu);
		}
		else if(type == CUBIC)
		{
			return cubicInterpolate(y0, y1, y2, y3, mu);
		}
		else if(type == HERMITE)
		{
			return hermiteInterpolate(y0, y1, y2, y3, mu, -0.25f, 0);
		}
		else return -1.0f;
	}

	float Interpolator::interpolateValue(std::vector<float> values, int length, int index, float mu, int type)
	{
		float y0, y1, y2, y3;
		y1 = values[index];

		if(index == 0)
		{
			y0 = values[index];
			y2 = values[index + 1];
			y3 = values[index + 2];
		}
		else if(index == (length - 2))
		{
			y0 = values[index - 1];
			y2 = values[index + 1];
			y3 = values[index + 1];
		}
		else if(index == (length - 1))
		{
			y0 = values[index - 1];
			y2 = values[index];
			y3 = values[index];
		}
		else
		{
			y0 = values[index - 1];
			y2 = values[index + 1];
			y3 = values[index + 2];
		}

		if(type == LINEAR)
		{
			return linearInterpolate(y1, y2, mu);
		}
		else if(type == CUBIC)
		{
			return cubicInterpolate(y0, y1, y2, y3, mu);
		}
		else if(type == HERMITE)
		{
			return hermiteInterpolate(y0, y1, y2, y3, mu, DEFAULT_TENSION, 0);
		}
		else return -1.0f;
	}

	// values wrap around at ends
	float Interpolator::wrappedInterpolateValue(float* values, int length, int index, float mu, int type)
	{	
		float y0, y1, y2, y3;
		y1 = values[index];

		if(index == 0)
		{
			y0 = values[length - 1];
			y2 = values[index + 1];
			y3 = values[index + 2];
		}
		else if(index == (length - 2))
		{
			y0 = values[index - 1];
			y2 = values[index + 1];
			y3 = values[0];
		}
		else if(index == (length - 1))
		{
			y0 = values[index - 1];
			y2 = values[0];
			y3 = values[1];
		}
		else
		{
			y0 = values[index - 1];
			y2 = values[index + 1];
			y3 = values[index + 2];
		}

		if(type == LINEAR)
		{
			return linearInterpolate(y1, y2, mu);
		}
		else if(type == CUBIC)
		{
			return cubicInterpolate(y0, y1, y2, y3, mu);
		}
		else if(type == HERMITE)
		{
			return hermiteInterpolate(y0, y1, y2, y3, mu, -0.25f, 0);
		}
		else return -1.0f;
	}

	float Interpolator::wrappedInterpolateValue(std::vector<float> values, int length, int index, float mu, int type)
	{	
		float y0, y1, y2, y3;
		y1 = values[index];

		if(index == 0)
		{
			y0 = values[length - 1];
			y2 = values[index + 1];
			y3 = values[index + 2];
		}
		else if(index == (length - 2))
		{
			y0 = values[index - 1];
			y2 = values[index + 1];
			y3 = values[0];
		}
		else if(index == (length - 1))
		{
			y0 = values[index - 1];
			y2 = values[0];
			y3 = values[1];
		}
		else
		{
			y0 = values[index - 1];
			y2 = values[index + 1];
			y3 = values[index + 2];
		}

		if(type == LINEAR)
		{
			return linearInterpolate(y1, y2, mu);
		}
		else if(type == CUBIC)
		{
			return cubicInterpolate(y0, y1, y2, y3, mu);
		}
		else if(type == HERMITE)
		{
			return hermiteInterpolate(y0, y1, y2, y3, mu, -0.25f, 0);
		}
		else return -1.0f;
	}

	float Interpolator::linearInterpolate(float y0, float y1, float mu)
	{
	   return (y0 * (1 - mu) + y1 * mu);
	}
}
