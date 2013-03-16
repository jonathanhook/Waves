/**
 * Class:	Point2f
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */
#ifndef Point2f_H
#define Point2f_H

namespace JDHUtility
{
	class Vector2f;

	class Point2f
	{
	public:
		Point2f(float x, float y);
		Point2f(const Point2f &position);
		Point2f(void);
		~Point2f(void);

		float	getX		(void) const;
		float	getY		(void) const;
		void	rotate		(float theta);
		void	setPosition	(const Point2f &position);
		void	setPosition	(float x, float y);
		void	scale		(float xs, float ys);
		void	scaleX		(float s);
		void	scaleY		(float s);
		void	setX		(float x);
		void	setY		(float y);
		void	translate	(float xt, float yt);
		void	translate	(const JDHUtility::Vector2f &tv);
		void	translateX	(float t);
		void	translateY	(float t);

	protected:
		float x, y;

	};
}
#endif