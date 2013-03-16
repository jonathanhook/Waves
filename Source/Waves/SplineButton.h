/**
 * Class:	SplineButton
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */
#ifndef SplineButton_H
#define SplineButton_H

#include "Button.h"

namespace Waves
{
	class SplineButton : 
		public Button
	{
	public:
		SplineButton(const Point2f &position, float width, float height, float *points, unsigned int pointCount);
		~SplineButton(void);

		void render(void);

	private:
		float *points;
		unsigned int pointCount;
		bool splineDirty;
		unsigned int SplineButtonDisplayList;
	};
}
#endif

