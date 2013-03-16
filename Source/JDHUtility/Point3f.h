/**
 * Class:	Point3f
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */
#ifndef Point3f_H
#define Point3f_H

#include "Point2f.h"

namespace JDHUtility
{
	class Point3f : 
		public Point2f
	{
	public:
		Point3f(float x, float y, float z);
		Point3f(void);
		~Point3f(void);

		float getZ(void) const;
		void scaleZ(float s);
		void setZ(float z);
		void translate(float xt, float yt, float zt);
		void translateZ(float t);
	protected:
		float z;

	};
}
#endif
