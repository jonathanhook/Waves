/**
 * Class:	Colour3f
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */
#ifndef Colour3f_H
#define Colour3f_H

#include <string>

namespace JDHUtility
{
	class Colour3f
	{
	public:
		Colour3f	(float r, float g, float b);
		Colour3f	(void);
		~Colour3f	(void);

		float	getR	(void) const;
		float	getG	(void) const;
		float	getB	(void) const;
		void	setR	(float r);
		void	setG	(float g);
		void	setB	(float b);
		void	use		(void) const;

	protected:
		float r, g, b;
	};
}
#endif