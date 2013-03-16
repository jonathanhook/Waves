/**
 * Class:	Vector2f
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */
#ifndef Vector2f_H
#define Vector2f_H

#include "JDHUtility/Point2f.h"

namespace JDHUtility
{
	class Vector2f :
		public Point2f
	{
	public:
		Vector2f(float x, float y);
		Vector2f(void);
		~Vector2f(void);

		float		getAngle		(const Vector2f &v) const;
		float		getDotProduct	(const Vector2f &v) const;
		Point2f		getCentre		(void) const;
		float		getCrossProduct	(const Vector2f &v) const;
		Vector2f	getCrossProduct	(void) const;
		float		getMagnitude	(void) const;
		Vector2f	getNormalised	(void) const;
		float		getScaleFactor	(const Vector2f &v) const;
		void		normalise		(void);
	};
}
#endif
