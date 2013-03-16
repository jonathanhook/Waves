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
#include "JDHUtility/GLContext.h"
#include "JDHUtility/GLMatrixf.h"

namespace JDHUtility
{
	/* Constructors */
	GLMatrixf::GLMatrixf(void) :
		Matrixf(OPEN_GL_MATRIX_DIM, OPEN_GL_MATRIX_DIM)
	{

	}

	GLMatrixf::~GLMatrixf(void)
	{

	}

	/* Public Member Functions */
	// only works with no rotation
	float GLMatrixf::getXScale(void) const
	{
		return matrix[0];
	}
	
	// only works with no rotation
	float GLMatrixf::getXTranslation(void) const
	{
		return matrix[12];
	}

	// only works with no rotation
	float GLMatrixf::getYScale(void) const
	{
		return matrix[5];
	}

	// only works with no rotation
	float GLMatrixf::getYTranslation(void) const
	{
		return matrix[13];
	}

	// only works with no rotation
	float GLMatrixf::getZScale(void) const
	{
		return matrix[10];
	}
	
	// only works with no rotation
	float GLMatrixf::getZTranslation(void) const
	{
		return matrix[14];
	}
	
	// un-tested
	void GLMatrixf::transform(Point3f &p) const
	{
		float a = p.getX();
		float b = p.getY();
		float c = p.getZ();
		float d = 1.0f;

		float vx = matrix[0] * a + matrix[4] * b + matrix[8]  *	c + matrix[12] * d;
		float vy = matrix[1] * a + matrix[5] * b + matrix[9]  *	c + matrix[13] * d;
		float vz = matrix[3] * a + matrix[6] * b + matrix[10] * c + matrix[14] * d;
	  //float vd = matrix[4] * a + matrix[7] * b + matrix[11] * c + matrix[15] * d;

		p.setX(vx);
		p.setY(vy);
		p.setZ(vz);
	}

	// only works with no rotation
	void GLMatrixf::unScalePoint(Point2f &p) const
	{
		p.scale(1.0f / getXScale(), 1.0f / getYScale());
	}

	// only works with no rotation
	void GLMatrixf::unTranslatePoint(Point2f &p) const
	{
		p.translateX(-getXTranslation());
		p.translateY(-getYTranslation());
	}
}
