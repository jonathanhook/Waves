/**
 * Class:	Colour3f
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */
#include "JDHUtility/GLWindowingSystem.h"
#ifdef GL_GLFW
#include <GL/glfw.h>
#endif 

#ifdef GL_GLUT
#include "JDHUtility/GlutInclude.h"
#endif

#include "JDHUtility/Colour3f.h"

namespace JDHUtility
{
	/* CONSTRUCTORS */
	Colour3f::Colour3f(float r, float g, float b)
	{
		Colour3f:: r = r;
		Colour3f:: g = g;
		Colour3f:: b = b;
	}

	Colour3f::Colour3f(void) 
	{
		Colour3f(0.0f, 0.0f, 0.0);
	}

	Colour3f::~Colour3f(void)
	{

	}

	/* PUBLIC MEMBER FUNCTIONS */
	float Colour3f::getR(void) const
	{
		return r;
	}

	float Colour3f::getG(void) const
	{
		return g;
	}

	float Colour3f::getB(void) const
	{
		return b;
	}

	void Colour3f::setR(float r)
	{
		this->r = r;
	}

	void Colour3f::setG(float g)
	{
		this->g = g;
	}

	void Colour3f::setB(float b)
	{
		this->b = b;
	}

	void Colour3f::use(void) const
	{
		glColor3f(r, g, b);
	}
}
