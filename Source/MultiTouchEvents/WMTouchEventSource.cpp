/**
 * Class:	WMTouchEventSource
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */
#define MOUSEEVENTF_FROMTOUCH 0xFF515700
#include <assert.h>
#include <stdio.h>
#include <Tpcshrd.h>
#include "WMTouchEventSource.h"

namespace MultiTouchEvents
{
	/* Static Variables */
	WNDPROC				WMTouchEventSource::prevWndProc = NULL;
	WMTouchEventSource	*WMTouchEventSource::instance	= NULL;

	/* Static Functions */
	LRESULT CALLBACK WMTouchEventSource::wmTouchProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		assert(prevWndProc);
		assert(instance);

		if(msg == WM_TABLET_QUERYSYSTEMGESTURESTATUS)
		{
			return	(TABLET_DISABLE_PRESSANDHOLD		|
					TABLET_DISABLE_PENTAPFEEDBACK		|
					TABLET_DISABLE_PENBARRELFEEDBACK	|
					TABLET_DISABLE_TOUCHUIFORCEON		|
					TABLET_DISABLE_TOUCHUIFORCEOFF		|
					TABLET_DISABLE_TOUCHSWITCH			|
					TABLET_DISABLE_FLICKS);

		}
		else if(msg == WM_TOUCH)
		{
			// WM_TOUCH message can contain several messages from different contacts
            // packed together.
            // Message parameters need to be decoded:
            unsigned int numInputs = (unsigned int) wParam; // Number of actual per-contact messages
            TOUCHINPUT* ti = new TOUCHINPUT[numInputs];		// Allocate the storage for the parameters of the per-contact messages
            if (ti == NULL)
            {
                return 0;
            }

            // Unpack message parameters into the array of TOUCHINPUT structures, each
            // representing a message for one single contact.
            if (GetTouchInputInfo((HTOUCHINPUT)lParam, numInputs, ti, sizeof(TOUCHINPUT)))
            {
                // For each contact, dispatch the message to the appropriate message
                // handler.
                for (unsigned int i = 0; i < numInputs; ++i)
                {
                    if (ti[i].dwFlags & TOUCHEVENTF_DOWN)
                    {
						instance->touchDown(&ti[i]);
                    }
                    else if (ti[i].dwFlags & TOUCHEVENTF_MOVE)
                    {
						instance->touchMove(&ti[i]);
                    }
                    else if (ti[i].dwFlags & TOUCHEVENTF_UP)
                    {
						instance->touchUp(&ti[i]);
                    }
                }
            }
            CloseTouchInputHandle((HTOUCHINPUT)lParam);
            delete [] ti;
		}
		else if((msg == WM_LBUTTONDOWN) && ((GetMessageExtraInfo() & MOUSEEVENTF_FROMTOUCH) == MOUSEEVENTF_FROMTOUCH))
		{
			// ignore clicks caused by touches
			return 0;
		}
		else
		{
			LRESULT result = CallWindowProc(prevWndProc, hWnd, msg, wParam, lParam);
			return result;
		}

		return NULL;
	}

	/* Constructors */
	WMTouchEventSource::WMTouchEventSource(HWND windowTarget) :
		EventSource(windowTarget)
	{

		isWMTouchDevice	= false;
		instance		= this;

		init();
	}

	WMTouchEventSource::~WMTouchEventSource(void)
	{
	}

	/* Private Member Functions */
	Point2f WMTouchEventSource::convert(TOUCHINPUT *ti)
	{
		float sx = ((float)GetSystemMetrics(SM_CXSCREEN) * 100);
		float sy = ((float)GetSystemMetrics(SM_CYSCREEN) * 100);
		sx = (float)ti->x / sx;
		sy = (float)ti->y / sy;

		Point2f p(sx, sy);
		if(windowTarget != NULL)
		{
			processPointForWindowTarget(p);
		}

		return p;
	}

	void WMTouchEventSource::touchDown(TOUCHINPUT *ti)
	{
		assert(ti);

		if(eventCallback != NULL) 
		{
			Point2f p = convert(ti);
			eventCallback(ti->dwID, p.getX(),p.getY(), FingerEventArgs::EventType::FINGER_ADDED);
		}
	}

	void WMTouchEventSource::touchMove(TOUCHINPUT *ti)
	{
		assert(ti);

		if(eventCallback != NULL) 
		{
			Point2f p = convert(ti);
			eventCallback(ti->dwID, p.getX(),p.getY(), FingerEventArgs::EventType::FINGER_UPDATED);
		}
	}
	
	void WMTouchEventSource::touchUp(TOUCHINPUT *ti)
	{
		assert(ti);

		if(eventCallback != NULL) 
		{
			Point2f p = convert(ti);
			eventCallback(ti->dwID, p.getX(),p.getY(), FingerEventArgs::EventType::FINGER_REMOVED);
		}
	}

	void WMTouchEventSource::init(void)
	{
		// setup proc
		assert(prevWndProc == NULL);
		prevWndProc = (WNDPROC)GetWindowLongPtr(windowTarget, GWL_WNDPROC);
		SetWindowLongPtr(windowTarget, GWL_WNDPROC, (LONG_PTR)wmTouchProc);
		
		// register for WM_TOUCH messages
		isWMTouchDevice = RegisterTouchWindow(windowTarget, TWF_FINETOUCH);

		if (isWMTouchDevice)
		{
#ifdef _DEBUG
			printf("Windows touch input not available on this window\n");
#endif
			false;
			return;
		}

		assert((bool)IsTouchWindow(windowTarget, NULL));

#ifdef _DEBUG
		printf("Windows touch input available, processing WM_TOUCH messages\n");
#endif
	}
}

