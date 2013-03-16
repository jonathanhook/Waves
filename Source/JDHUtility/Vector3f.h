/**
 * Class:	Vector3f
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */
#ifndef Vector3f_H
#define Vector3f_H

#include "Point3f.h"

namespace JDHUtility
{
	class Vector3f :
		public Point3f
	{
	public:
		Vector3f(float x, float y, float z);
		Vector3f(void);
		~Vector3f(void);
	};
}
#endif
