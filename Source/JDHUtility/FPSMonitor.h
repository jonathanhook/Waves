/**
 * Class:	FPSMonitor
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */
#ifndef FPSMonitor_H
#define FPSMonitor_H

#include "JDHUtility/RollingAverage.h"

namespace JDHUtility
{
	class FPSMonitor
	{
	public:
		FPSMonitor(void);
		~FPSMonitor(void);

		float getFPS(void) const;
		void markRender(unsigned int time);

	private:
		RollingAverage<int> *average;
		unsigned int last;
	};
}
#endif

