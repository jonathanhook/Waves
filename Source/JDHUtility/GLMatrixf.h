/**
 * Class:	GLMatrix
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 *
 * Matrix in the form:
 * [ m0	m4	m8	m12	]
 * [ m1	m5	m9	m13	]
 * [ m2	m6	m10 m14	]
 * [ m3	m7	m11	m15	]
 */
#pragma once

#include "JDHUtility/Matrixf.h"
#include "JDHUtility/Point2f.h"
#include "JDHUtility/Point3f.h"

namespace JDHUtility
{
	class GLMatrixf :
		public Matrixf
	{
	public:
		static const unsigned int OPEN_GL_MATRIX_DIM = 4;

		GLMatrixf	(void);
		~GLMatrixf	(void);

		float	getXScale		(void) const;
		float	getXTranslation	(void) const;
		float	getYScale		(void) const;
		float	getYTranslation	(void) const;
		float	getZScale		(void) const;
		float	getZTranslation	(void) const;
		void	transform		(Point2f &p) const;
		void	transform		(Point3f &p) const;
		void	unScalePoint	(Point2f &p) const;
		void	unTranslatePoint(Point2f &p) const;
	};
}

