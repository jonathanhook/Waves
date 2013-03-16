/**
 * Class:	DebugTimer
 * Author:	Jonathan David Hook & Dan Jackson
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */
#include <assert.h>
#include "JDHUtility/CrossPlatformTime.h"
#include "JDHUtility/DebugTimer.h"

namespace JDHUtility
{
	/* Private Constants */
	const std::string DebugTimer::PRINT_FORMAT	= "%s: %i\n";

	/* Private Static Variables */
	DebugTimer *DebugTimer::instance = NULL;

	/* Public Static Member Functions */
	void DebugTimer::end(const std::string &key)
	{
#ifdef _DEBUG

		DebugTimer *d = DebugTimer::getInstance();
		assert(d);
		assert(d->values.count(key) > 0);

		unsigned int time = CrossPlatformTime::getTimeMillis();
		TimePeriod *p = &d->values[key];
		p->end = time;

#endif		
	}

	void DebugTimer::printAll(void)
	{
#ifdef _DEBUG

		DebugTimer *d = DebugTimer::getInstance();
		assert(d);

		printf("Debug timings (ms):\n");
		std::map<std::string, TimePeriod>::iterator it;
		for(it = d->values.begin(); it != d->values.end(); it++)
		{
			const	std::string *s	= &(*it).first;
			const	TimePeriod *p	= &(*it).second;
			int		elapsed			= p->end - p->start;

			if(elapsed >= 0)
			{
				printf(PRINT_FORMAT.c_str(), s->c_str(), elapsed);  
			}
		}
		printf("\n");

#endif
	}

	void DebugTimer::start(const std::string &key)
	{
#ifdef _DEBUG
		
		DebugTimer *d = DebugTimer::getInstance();
		assert(d);

		unsigned int time = CrossPlatformTime::getTimeMillis();

		if(d->values.count(key) > 0)
		{
			TimePeriod *p = &d->values[key];
			p->start = time;
		}
		else
		{
			TimePeriod p;
			p.start = time;
			p.end	= 0;

			d->values[key] = p;
		}

#endif
	}
	
	/* Private Static Member Functions */
	DebugTimer *DebugTimer::getInstance(void)
	{
		if(instance == NULL)
		{
			instance = new DebugTimer();
		}

		return (DebugTimer *)instance;
	}

	/* Constructors */
	DebugTimer::DebugTimer(void)
	{
	}

	DebugTimer::~DebugTimer(void)
	{
		values.clear();
	}
}
