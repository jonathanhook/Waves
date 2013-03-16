/**
 * Class:	WIN32Utils
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */
#ifndef WIN32Utils_H
#define WIN32Utils_H

#ifdef _WIN32
#define WINVER 0x0501   // xp plain
#define WIN_LEAN_AND_MEAN
#include <windows.h>
#else 
#error WIN32Utils is not implemented for this platform
#endif

namespace JDHUtility
{
	class Vector2f;

	class WIN32Utils
	{
	public:
		static Vector2f getSecondMonitorBounds(void);
		static void screenPositionToWindowPosition(float x, float y, float &wx, float &wy, const HWND &hWnd, bool squarePixel);
		static void setWindowBorder(const HWND &hWnd, bool hasBorder);

	private:
		WIN32Utils::WIN32Utils(void);
		WIN32Utils::~WIN32Utils(void);
	};
}
#endif

