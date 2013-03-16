/**
 * Class:	Colour4f
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */
#ifndef Colour4f_H
#define Colour4f_H

#include "JDHUtility/Colour3f.h"

namespace JDHUtility
{
	class Colour4f : 
		public Colour3f
	{
	public:
		Colour4f	(float r, float g, float b, float a);
		Colour4f	(void);
		~Colour4f	(void);
	
		float	getA	(void) const;
		void	setA	(float a);
		void	use		(void) const;

	protected:
		float a;

	};
}
#endif
