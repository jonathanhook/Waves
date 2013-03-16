/**
 * Class:	WGLStuff
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */
#include "GLWindowingSystem.h"
#ifdef GL_GLFW
#include <GL/glfw.h>
#endif 

#ifdef GL_GLUT
#include "GlutInclude.h"
#endif

#include "WGLStuff.h"

namespace JDHUtility
{
	typedef BOOL (APIENTRY *PFNWGLSWAPINTERVALFARPROC)( int );
	PFNWGLSWAPINTERVALFARPROC wglSwapIntervalEXT = 0;

	bool WGLStuff::setVSync(int interval)
	{
		const char *extensions = (const char *)glGetString(GL_EXTENSIONS);

		if(strstr(extensions, "WGL_EXT_swap_control") == 0)
		{
			return false;
		}
		else
		{
			wglSwapIntervalEXT = (PFNWGLSWAPINTERVALFARPROC)wglGetProcAddress( "wglSwapIntervalEXT" );

			if(wglSwapIntervalEXT)
			{
				wglSwapIntervalEXT(interval);
			}

			return true;
		}
	}

}
