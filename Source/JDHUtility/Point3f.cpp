/**
 * Class:	Point3f
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */
#include "JDHUtility/Point3f.h"

namespace JDHUtility
{
	Point3f::Point3f(float x, float y, float z) :
		Point2f(x, y)
	{
		this->z = z;
	}

	Point3f::Point3f(void)
	{
		Point3f(0.0f, 0.0f, 0.0f);
	}

	Point3f::~Point3f(void)
	{

	}

	float Point3f::getZ(void) const
	{
		return z;
	}

	void Point3f::scaleZ(float s)
	{
		z *= s;
	}

	void Point3f::setZ(float z)
	{
		this->z = z;
	}

	void Point3f::translate(float xt, float yt, float zt)	
	{
		Point2f::translate(xt, yt);
		translateZ(zt);
	}

	void Point3f::translateZ(float t)
	{
		z += t;
	}
}