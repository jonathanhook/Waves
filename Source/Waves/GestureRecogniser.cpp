/**
 * Class:	GestureRecogniser
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */
#include <map>
#include <math.h>
#include "JDHUtility/GlutInclude.h"
#include "GestureRecogniser.h"

namespace Waves
{
	/* STATIC VARS */
	int GestureRecogniser::GR_DWELL_MS = 10;
	float GestureRecogniser::GR_MAXDISTANCE = 0.1f;
	float GestureRecogniser::GR_MOVE_TOLERANCE = 0.005f;

	/* PUBLIC STATIC FUNCTIONS */
	bool GestureRecogniser::stationaryFinger(const Finger &finger, Point2f &position, int dwellMs)
	{
		if(hasntMoved(finger) && isDwelling(finger, dwellMs))
		{
			position.setX(finger.getAnchor().getX());
			position.setY(finger.getAnchor().getY());
			return true;
		}	

		position = Point2f(0.0f, 0.0f);
		return false;
	}

	bool GestureRecogniser::stationaryFinger(std::map<int, Finger*> fingers, Point2f &position, int dwellMs)
	{
		if(fingers.size() >= 1)
		{
			std::map<int, Finger *>::iterator it; 
			for(it = fingers.begin(); it != fingers.end(); it++)
			{
				Finger *f = (*it).second;
				assert(f != NULL);

				if(hasntMoved(*f) && isDwelling(*f, dwellMs))
				{
					position.setX(f->getAnchor().getX());
					position.setY(f->getAnchor().getY());
					return true;
				}
			}
		}	

		position = Point2f(0.0f, 0.0f);
		return false;
	}

	bool GestureRecogniser::twoStationaryFingers(std::map<int, Finger*> fingers, Point2f &position, int dwellMs)
	{
		if(fingers.size() >= 2)
		{
			std::map<int, Finger *>::iterator it0; 
			std::map<int, Finger *>::iterator it1; 

			for(it0 = fingers.begin(); it0 != fingers.end(); it0++)
			{
				for(it1 = fingers.begin(); it1 != fingers.end(); it1++)
				{
					// if two fingers which havent moved since they've been put down are detected...
					if(it0 != it1 && areClose(*(*it0).second, *(*it1).second) && 
						hasntMoved(*(*it0).second) && 
						hasntMoved(*(*it1).second) &&
						isDwelling(*(*it0).second, dwellMs) &&
						isDwelling(*(*it1).second, dwellMs))
					{
						position = getLeftMost(*(*it0).second, *(*it1).second).getLast();
						return true;
					}
				}
			}
		}

		position = Point2f(0.0f, 0.0f);
		return false;
	}

	/* PRIVATE STATIC FUNCTIONS */
	bool GestureRecogniser::areClose(const Finger &f0, const Finger &f1)
	{
		float xd = f1.getLast().getX() - f0.getLast().getX();
		float yd = f1.getLast().getY() - f0.getLast().getY();
		return sqrt((xd * xd) + (yd * yd)) <= GR_MAXDISTANCE;
	}

	const Finger &GestureRecogniser::getLeftMost(const Finger &f0, const Finger &f1)
	{
		if(f0.getLast().getX() <= f1.getLast().getX()) return f0;
		else return f1;
	}

	bool GestureRecogniser::hasntMoved(const Finger &f)
	{
		return (abs(f.getAnchor().getX() - f.getLast().getX()) < GR_MOVE_TOLERANCE) && (abs(f.getAnchor().getY() - f.getLast().getY()) < GR_MOVE_TOLERANCE);
	}

	bool GestureRecogniser::isDwelling(const Finger &f, int dwellMs)
	{
		int currentTime = glutGet(GLUT_ELAPSED_TIME);
		int elapsedTime = currentTime - f.getTimestamp();
		return elapsedTime >= dwellMs;
	}
}