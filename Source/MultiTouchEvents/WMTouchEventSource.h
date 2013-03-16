/**
 * Class:	WMTouchEventSource
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */
#pragma once
#include "EventSource.h"

namespace MultiTouchEvents
{
	class WMTouchEventSource :
		public EventSource
	{
	public:
		WMTouchEventSource	(HWND hWnd);
		~WMTouchEventSource	(void);

		bool getIsWMTouchDevice(void) const;

	private:
		static	WMTouchEventSource *instance;
		static	WNDPROC prevWndProc;
		static	LRESULT CALLBACK	wmTouchProc	(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

		bool	isWMTouchDevice;

		Point2f convert	(TOUCHINPUT *ti);
		void init		(void);
		void touchDown	(TOUCHINPUT *ti);
		void touchMove	(TOUCHINPUT *ti);
		void touchUp	(TOUCHINPUT *ti);


	};
}

