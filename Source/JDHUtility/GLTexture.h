/**
 * Class:	GLTexture
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */
#ifndef GLTexture_H
#define GLTexture_H

#include "JDHUtility/GLWindowingSystem.h"
#ifdef GL_GLFW
#include <GL/glfw.h>
#endif 

#ifdef GL_GLUT
#include "JDHUtility/GlutInclude.h"
#endif

#include <string>

namespace JDHUtility
{
	class GLTexture
	{
	public:
		GLTexture(std::string path);
		~GLTexture(void);

		void			bind(	GLenum mode			= GL_MODULATE, 
								GLenum minFilter	= GL_NEAREST, 
								GLenum magFilter	= GL_LINEAR, 
								GLenum wrapS		= GL_CLAMP, 
								GLenum wrapT		= GL_CLAMP) const;
		unsigned int	getId(void) const;
		bool			isTexture(void) const;

	private:
		static const unsigned int INVALID_TEXTURE;

		unsigned int id;
	};
}
#endif

