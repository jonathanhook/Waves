/**
 * Class:	MotionPath
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */
#ifndef MotionPath_H
#define MotionPath_H

namespace JDHUtility { class Point3f; }

using namespace JDHUtility;

namespace WavesVisuals
{
	class MotionPath
	{
	public:
		MotionPath(unsigned int numPoints = DEFAULT_NUM_POINTS, 
					float xMax = DEFAULT_X_MAX, 
					float yMax = DEFAULT_Y_MAX, 
					float zMax = DEFAULT_Z_MAX, 
					float distanceBetweenPoints = DEFAULT_DISTANCE_BETWEEN_POINTS);
		~MotionPath(void);

		const Point3f &getCurrentValue(void) const;
		unsigned int getNumPoints(void) const;
		const Point3f &getPoint(unsigned int index) const;
		void updatePosition(float increment);

	private:
		static const float DEFAULT_DISTANCE_BETWEEN_POINTS;
		static const unsigned int DEFAULT_NUM_POINTS = 50;
		static const float DEFAULT_X_MAX;
		static const float DEFAULT_Y_MAX;
		static const float DEFAULT_Z_MAX;

		Point3f *currentValue;
		float distanceBetweenPoints;
		unsigned int numPoints;
		Point3f *points;
		float position;
		float xMax;
		float yMax; 
		float zMax;

		void initPath(void);
	};
}
#endif

