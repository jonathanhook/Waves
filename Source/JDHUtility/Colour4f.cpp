/**
 * Class:	Colour4f
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

#include "JDHUtility/Colour4f.h"

namespace JDHUtility
{
	/* CONSTRUCTORS */
	Colour4f::Colour4f(float r, float g, float b, float a) :
		Colour3f(r, g, b)
	{
		Colour4f::a = a;
	}

	Colour4f::Colour4f(void)
	{
		Colour4f(0.0f, 0.0f, 0.0f, 0.0f);
	}

	Colour4f::~Colour4f(void)
	{

	}

	/* PUBLIC MEMBER FUNCTIONS */
	float Colour4f::getA() const
	{
		return a;
	}

	void Colour4f::setA(float a)
	{
		this->a = a;
	}

	void Colour4f::use(void) const
	{
		glColor4f(r, g, b, a);

	}
}
