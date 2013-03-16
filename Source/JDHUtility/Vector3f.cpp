/**
 * Class:	Vector3f
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */
#include "Vector3f.h"

namespace JDHUtility
{
	/* CONSTRUCTORS */
	Vector3f::Vector3f(float x, float y, float z) :
		Point3f(x, y, z)
	{
	}

	Vector3f::Vector3f(void) :
		Point3f()
	{
	}

	Vector3f::~Vector3f(void)
	{
	}
}
