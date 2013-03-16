/**
 * Class:	GLUtils
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */
#include "JDHUtility/GLWindowingSystem.h"
#ifdef GL_GLFW
#include <GL/glfw.h>
#endif 

#ifdef GL_GLUT
#include "GlutInclude.h"
#endif

#include "JDHUtility/GLUtils.h"

namespace JDHUtility
{
	void GLUtils::billboardBegin() 
	{
		float modelview[16];
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();

		glGetFloatv(GL_MODELVIEW_MATRIX , modelview);

		for(unsigned int i = 0; i < 3; i++)
		{
			for(unsigned int j = 0; j < 3; j++)
			{
				if (i == j)
				{
					modelview[i * 4 + j] = 1.0f;
				}
				else
				{
					modelview[i * 4 + j] = 0.0f;
				}
			}
		}

		glLoadMatrixf(modelview);
	}

	void GLUtils::billboardEnd() 
	{
		glPopMatrix();
	}
}