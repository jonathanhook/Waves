/**
 * Class:	DebugTimer
 * Author:	Jonathan David Hook & Dan Jackson
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */
#pragma once
#include <map>
#include <string>
#include "Singleton.h"

namespace JDHUtility
{
	class DebugTimer
	{
	public:
		static void end		(const std::string &key);
		static void printAll(void);
		static void start	(const std::string &key);

	private:
		static const std::string PRINT_FORMAT;

		struct TimePeriod
		{
			unsigned int start;
			unsigned int end;
		};

		static DebugTimer *getInstance(void);

		static DebugTimer *instance;

		DebugTimer(void);
		~DebugTimer(void);

		std::map<std::string, TimePeriod> values;
	};
}

