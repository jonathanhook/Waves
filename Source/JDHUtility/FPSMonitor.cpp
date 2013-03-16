/**
 * Class:	FPSMonitor
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */
#include <assert.h>
#include "JDHUtility/FPSMonitor.h"

namespace JDHUtility
{
	/* CONSTRUCTORS */
	FPSMonitor::FPSMonitor(void)
	{
		average = new RollingAverage<int>();
		last = 0;
	}

	FPSMonitor::~FPSMonitor(void)
	{
		if(average != NULL)
		{
			delete average;
			average = NULL;
		}
	}

	/* PUBLIC MEMBER FUNCTIONS */
	float FPSMonitor::getFPS(void) const
	{
		assert(average != NULL);

		unsigned int ms = average->getAverage();
		if(ms == 0) 
		{
			return 0;
		}
		else
		{
			return 1000.0f / (float)ms;
		}
	}

	void FPSMonitor::markRender(unsigned int time)
	{
		assert(average != NULL);

		if(last != 0)
		{
			average->addValue(time - last);
		}
		last = time;
	}
}
