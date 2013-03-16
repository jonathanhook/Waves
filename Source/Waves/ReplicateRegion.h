/* Waves -- ReplicateRegion
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		homepages.cs.ncl.ac.uk/j.d.hook
 */
#ifndef ReplicateRegion_H
#define ReplicateRegion_H

#include <map>
#include "Finger.h"
#include "InterfaceElement.h"

namespace Waves
{
	class ReplicateRegion :
		public InterfaceElement
	{
	public:
		struct RegionPointer
		{
			int nextPointer;
			int prevPointer;
			float width;
			float x;
			RegionPointer()
			{
				nextPointer = 0;
				prevPointer = 0;
				width = 0.0f;
				x = 0.0f;
			}
		};

		ReplicateRegion(const Point2f &position, float width, float height);
		~ReplicateRegion(void);

		static const int FINGER_LIFTED = -1;
		static const int FINGER_TOUCHED = 1;

		static float BORDER_OPACITY;
		static float OPACITY;

		int next(RegionPointer &pointer);
		int prev(RegionPointer &pointer);
		void render(void);
		void setX(float x);
		void setWidth(float width);

	private:
		int getMax(void);
		int getStartVal(void);
	};
}
#endif
