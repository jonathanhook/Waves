/**
 * Class:	Point2f
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */
#include <math.h>
#include "JDHUtility/Point2f.h"
#include "JDHUtility/Vector2f.h"

namespace JDHUtility
{
	/* CONSTRUCTORS */
	Point2f::Point2f(float x, float y)
	{
		this->x = x;
		this->y = y;
	}

	Point2f::Point2f(void)
	{
		Point2f(0.0f, 0.0f);
	}

	Point2f::Point2f(const Point2f &position)
	{
		this->x = position.getX();
		this->y = position.getY();
	}

	Point2f::~Point2f(void)
	{

	}

	/* PUBLIC MEMBER FUNCTIONS */
	float Point2f::getX(void) const
	{
		return x;
	}

	float Point2f::getY(void) const
	{
		return y;
	}
		
	void Point2f::rotate(float theta)
	{
		float x1 = cos(theta) * x - sin(theta) * y;
		float y1 = sin(theta) * x + cos(theta) * y;

		x = x1;
		y = y1;
	}

	void Point2f::setPosition(const Point2f &position)
	{
		this->x = position.getX();
		this->y = position.getY();
	}

	void Point2f::setPosition(float x, float y)
	{
		this->x = x;
		this->y = y;
	}

	void Point2f::scale(float xs, float ys)
	{
		scaleX(xs);
		scaleY(ys);
	}
		
	void Point2f::scaleX(float s)
	{
		x *= s;
	}

	void Point2f::scaleY(float s)
	{
		y *= s;
	}

	void Point2f::setX(float x)
	{
		this->x = x;
	}
		
	void Point2f::setY(float y)
	{
		this->y = y;
	}

	void Point2f::translate(float xt, float yt)
	{
		translateX(xt);
		translateY(yt);
	}

	void Point2f::translate(const Vector2f &tv)
	{
		translateX(tv.getX());
		translateY(tv.getY());
	}

	void Point2f::translateX(float t)
	{
		x += t;
	}

	void Point2f::translateY(float t)
	{
		y += t;
	}
}
