/**
 * CrossPlatformTime -- Returns the time in milliseconds on Windows and UNIX based systems
 */
#ifdef _WIN32
#define _CRT_SECURE_NO_DEPRECATE
#include <windows.h>
#pragma comment(lib, "winmm")
#else
#include <time.h>
#endif
#include "JDHUtility/CrossPlatformTime.h"

namespace JDHUtility
{
	unsigned int CrossPlatformTime::getTimeMillis(void)
	{
#ifdef _WIN32
		return (unsigned int)timeGetTime(); 
#else
		unsigned int sec, usec;
#if defined(HAVE_CLOCK_GETTIME) && defined(CLOCK_PROCESS_CPUTIME_ID)
		struct timespec tp;
		clock_gettime(CLOCK_REALTIME, &tp);
		usec = (tp.tv_nsec + 500) / 1000;
#else
		struct timeval tp;
		gettimeofday(&tp, NULL);
		usec = tp.tv_usec;
#endif
		sec = tp.tv_sec;
		return sec * 1000 + usec / 1000;
#endif
	}
}

