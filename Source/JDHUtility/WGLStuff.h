/**
 * Class:	WGLStuff
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */
#ifndef WGLStuff_H
#define WGLStuff_H

#include <Windows.h>

namespace JDHUtility
{
	class WGLStuff
	{
	public:
		static bool WGLStuff::setVSync(int interval = 1);
	};
}
#endif

