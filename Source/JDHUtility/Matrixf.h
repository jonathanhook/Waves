/**
 * Class:	Matrixf
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 *
 * Matrix in the form:
 * m1 m4 m7 
 * m2 m5 m8
 * m3 m6 m9
 */
#include "Point2f.h"

#ifndef Matrixf_H
#define Matrixf_H

namespace JDHUtility
{
	class Matrixf
	{
	public:
		Matrixf(unsigned int width, unsigned int height);
		~Matrixf(void);

		Matrixf *add(Matrixf &m) const;
		unsigned int getHeight(void) const;
		float *getMatrix(void);
		float getValue(unsigned int x, unsigned int y) const;
		unsigned int getWidth(void) const;
		Matrixf *multiply(Matrixf &m) const;
		void setValue(float value, unsigned int x, unsigned int y);

	protected:
		Matrixf(void);

		unsigned int width;
		float *matrix;
		unsigned int height;
	};
}
#endif

