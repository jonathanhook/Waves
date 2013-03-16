/**
 * Class:	GestureRecogniser
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */
#ifndef GestureRecogniser_H
#define GestureRecogniser_H

#include <map>
#include "Finger.h"
#include "JDHUtility/Point2f.h"

using namespace JDHUtility;

namespace Waves
{
	class GestureRecogniser
	{
	public:
		static bool stationaryFinger(const Finger &finger, Point2f &position, int dwellMs = GR_DWELL_MS); 
		static bool stationaryFinger(std::map<int, Finger*> fingers, Point2f &position, int dwellMs = GR_DWELL_MS); 
		static bool twoStationaryFingers(std::map<int, Finger*> fingers, Point2f &position, int dwellMs = GR_DWELL_MS);

	private:
		static int GR_DWELL_MS;
		static float GR_MAXDISTANCE;
		static float GR_MOVE_TOLERANCE;
	
		static bool areClose(const Finger &f0, const Finger &f1);
		static const Finger &getLeftMost(const Finger &f0, const Finger &f1);
		static bool hasntMoved(const Finger &f);
		static bool isDwelling(const Finger &f, int dwellMs);
	};
}
#endif
