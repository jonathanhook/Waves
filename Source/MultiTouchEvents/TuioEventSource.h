/**
 * Class:	TuioEventSource
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */
#ifndef TuioEventSource_H
#define TuioEventSource_H

// work-around for TUIO_CPP win32 definition error
#ifdef _WIN32
#define WIN32
#endif

#include <windows.h>
#include "EventSource.h"
#include "TuioClient.h"
#include "TuioListener.h"

using namespace TUIO;

namespace MultiTouchEvents
{
	class TuioEventSource : 
		public EventSource, 
		public TuioListener
	{
	public:
		static const unsigned int DEFAULT_PORT = 3333;

		TuioEventSource(const HWND &windowTarget, unsigned int port = DEFAULT_PORT);
		~TuioEventSource(void);

		void addTuioCursor(TUIO::TuioCursor *tcur);
		void addTuioObject(TUIO::TuioObject *tobj);
		void refresh(TUIO::TuioTime ftime);
		void removeTuioCursor(TUIO::TuioCursor *tcur);
		void removeTuioObject(TUIO::TuioObject *tobj);
		void updateTuioCursor(TUIO::TuioCursor *tcur);
		void updateTuioObject(TUIO::TuioObject *tobj);

	private:
		TUIO::TuioClient *tuioClient;
	};
}
#endif
