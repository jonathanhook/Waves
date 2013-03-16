/**
 * Class:	Vector2f
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */
#include <math.h>
#include "JDHUtility/Point2f.h"
#include "JDHUtility/Vector2f.h"

namespace JDHUtility
{
	/* CONSTRUCORS */
	Vector2f::Vector2f(float x, float y) :
		Point2f(x, y)
	{
	}

	Vector2f::Vector2f(void) :
		Point2f()
	{
	}

	Vector2f::~Vector2f(void)
	{
	}

	/* PUBLIC MEMBER FUNCTIONS */
	float Vector2f::getAngle(const Vector2f &v) const
	{
		// WARNING: this may not work

		float length = (getMagnitude() * v.getMagnitude());
        if (length == 0.0f)
        {
            return 0.0f;
        }
        
		return (float)acos(getDotProduct(v) / length);
	}
	
	float Vector2f::getDotProduct(const Vector2f &v) const
	{
		return (x * v.getX()) + (y * v.getY());
	}

	float Vector2f::getCrossProduct(const Vector2f &v) const
	{	
		float vx = v.getX();
		float vy = v.getY();

		return (x * vy) - (y * vx);
	}

	Vector2f Vector2f::getCrossProduct(void) const
	{
		return Vector2f(y, -x);
	}

	Point2f Vector2f::getCentre(void) const
	{
		return Point2f(x / 2.0f, y / 2.0f); 
	}

	float Vector2f::getMagnitude(void) const
	{
		return sqrt((x * x) + (y * y));
	}

	Vector2f Vector2f::getNormalised(void) const
	{
		float magnitude = getMagnitude();
		
		if(magnitude == 0.0f)
		{
			return Vector2f(0.0f, 0.0f);
		}

		return Vector2f(x / magnitude, y /magnitude);
	}

	float Vector2f::getScaleFactor(const Vector2f &v) const
	{
		return getMagnitude() / v.getMagnitude();
	}

	void Vector2f::normalise(void)
	{
		float magnitude = getMagnitude();
		x /= magnitude;
		y /= magnitude;
	}
}
