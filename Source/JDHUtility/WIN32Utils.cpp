/**
 * Class:	WIN32Utils
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */
#include <stdio.h>
#include "JDHUtility/Vector2f.h"
#include "JDHUtility/WIN32Utils.h"

namespace JDHUtility
{
	/* STATIC MEMBER FUNCTIONS */
	Vector2f WIN32Utils::getSecondMonitorBounds(void)
	{
		Vector2f bounds;
		unsigned int primaryScreenWidth = GetSystemMetrics(SM_CXSCREEN);

		POINT p;
		p.x = (primaryScreenWidth + 1);
		p.y = 0;

		HMONITOR secondMonitor = MonitorFromPoint(p, MONITOR_DEFAULTTONULL);
		MONITORINFO mi;
		mi.cbSize = sizeof(MONITORINFO);
		BOOL result = GetMonitorInfo(secondMonitor, &mi);

		if(result)
		{
			RECT rcMonitor = mi.rcMonitor;
			float dx = rcMonitor.right;
			float dy = rcMonitor.bottom;

			bounds.setX(dx - primaryScreenWidth);
			bounds.setY(dy);
		}

		return bounds;
	}

	void WIN32Utils::screenPositionToWindowPosition(float x, float y, float &wx, float &wy, const HWND &hWnd, bool squarePixel)
	{
		RECT rect;
		BOOL result = GetWindowRect(hWnd, &rect);

		RECT clientRect;
		BOOL clientResult = GetClientRect(hWnd, &clientRect);

		if(result && clientResult)
		{
			HMONITOR hMonitor = MonitorFromRect(&rect, MONITOR_DEFAULTTONEAREST);

			MONITORINFO mi;
			mi.cbSize = sizeof(MONITORINFO);

			result = GetMonitorInfo(hMonitor, &mi);

			if(result)
			{
				RECT rcMonitor = mi.rcMonitor;

				int mx = (int)(x * (float)(rcMonitor.right - rcMonitor.left));
				int my = (int)(y * (float)rcMonitor.bottom);

				int iwx = (mx - (rect.left - rcMonitor.left)) - ((rect.right - rect.left) - clientRect.right);
				int iwy = (my - rect.top) - ((rect.bottom - rect.top) - clientRect.bottom);

				wx = (float)iwx / (float)clientRect.right;
				wy = (float)iwy / (float)clientRect.bottom;

				if(squarePixel)
				{
					float ratio = (float)clientRect.right / (float)clientRect.bottom;
					wy = wy / ratio;
				}
			}
		}
	}

	void WIN32Utils::setWindowBorder(const HWND &hWnd, bool hasBorder)
	{
		DWORD dwStyle = GetWindowLong(hWnd, GWL_STYLE);
		if(hasBorder)
		{
			dwStyle &= ~(WS_CAPTION | WS_THICKFRAME | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_SYSMENU | WS_POPUP);
			dwStyle |= WS_CAPTION | WS_THICKFRAME | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_SYSMENU | WS_POPUP;
			SetWindowLong(hWnd, GWL_STYLE, dwStyle);
		}
		else
		{
			dwStyle &= ~(WS_CAPTION | WS_THICKFRAME | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_SYSMENU | WS_POPUP);
			dwStyle |= WS_SYSMENU;
			SetWindowLong(hWnd, GWL_STYLE, dwStyle);			
		}
	}
}
