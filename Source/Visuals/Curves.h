/**
 * Class:	Curves
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */
#ifndef Curves_H
#define Curves_H

#include "Visuals/Visual.h"

namespace WavesVisuals
{
	class MotionPath;

	class Curves :
		public Visual
	{
	public:
		typedef struct
		{

			Point3f centre;
			bool enabled;
			float length;
			float X;
			float Y;
			float Z;
		} LineProfile;

		Curves(unsigned int maxNumLines = DEFAULT_MAX_NUM_LINES);
		~Curves(void);

		void draw(void);
		Visual *getInstance(void) const;
		void update(void);

	private:
		static const unsigned int DEFAULT_MAX_NUM_LINES = 250;
		static const std::string LINE_LENGTH_NAME;
		static const std::string LINE_SPEED_NAME;
		static const float MAX_LINE_LENGTH;
		static const float MAX_MOTION_PATH_INCREMENT;
		static const float MAX_OPACITY;
		static const unsigned int UPDATE_RATE_MS = 16;

		bool firstLine;
		unsigned int last;
		LineProfile *lines;
		unsigned int linePtr;
		unsigned int maxNumLines;
		MotionPath *motionPath;
		float sinVal;

		void addLine(void);
		void rotatePoint(Point3f &p, float ax, float ay, float az) const;
	};
}
#endif

