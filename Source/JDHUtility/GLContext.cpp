/**
 * Class:	GLContext
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

#include <assert.h>
#include <exception>
#include "JDHUtility/GLContext.h"

namespace JDHUtility
{
	/* STATIC VARS */
	const std::string				GLContext :: DEFAULT_FONT = "Visitor TT1 BRK";
	std::map<HGLRC, GLContext *>	GLContext :: instances;

	/* PUBLIC STATIC FUNCTIONS */
	GLContext *GLContext :: getCurrentContext(void)
	{
		HGLRC currentContext = lookupCurrentContext();
		if(instances.count(currentContext) == 0)
		{
			throw new std::exception("GLContext Error: not initialised");
		}

		return instances[currentContext];
	}

	GLContext *GLContext :: initialiseCurrentContext(std::string font)
	{
		HGLRC currentContext = lookupCurrentContext();
		instances[currentContext] = new GLContext(font);
		return instances[currentContext];
	}

	/* PRIVATE STATIC FUNCTIONS */
	HGLRC GLContext :: lookupCurrentContext(void)
	{
		return wglGetCurrentContext();
	}

	/* CONSTRUCTORS */
	GLContext :: GLContext(std::string font)
	{
		fontRenderer = new GLFontRenderer(font);
	}

	GLContext :: ~GLContext(void)
	{
		for(std::map<std::string, GLTexture *>::iterator it = textures.begin(); it != textures.end(); it++)
		{
			if((*it).second != NULL)
			{
				delete (*it).second;
				(*it).second = NULL;
			}
		}
		textures.clear();
	}

	/* PUBLIC MEMBER FUNCTIONS */
	void GLContext :: bindTexture(const std::string &path)
	{
		if(textures.count(path) > 0)
		{
			std::map<std::string, GLTexture *>::const_iterator it = textures.find(path);
			GLTexture *tex = (*it).second;
			assert(tex != NULL);

			tex->bind();
		}
		else
		{
			GLTexture *tex = loadTexture(path);
			assert(tex != NULL);

			tex->bind();
		}
	}

	int GLContext :: getDisplayList(const std::string &key) const
	{
		if(displayLists.count(key) > 0)
		{
			std::map<std::string, unsigned int>::const_iterator it = displayLists.find(key);
			unsigned int dl = (*it).second;
			return dl;
		}
		else
		{
			return -1;
		}
	}

	int GLContext :: getVertexBuffer(const std::string &key) const
	{
		if(vertexBuffers.count(key) > 0)
		{
			std::map<std::string, unsigned int>::const_iterator it = vertexBuffers.find(key);
			unsigned int vbo = (*it).second;
			return vbo;
		}
		else
		{
			return -1;
		}
	}

	void GLContext :: renderString(std::string s, const Vector2f &box, float border) const
	{
		assert(fontRenderer != NULL);

		fontRenderer->renderString(s, box, border);
	}

	GLTexture *GLContext :: loadTexture(const std::string &path)
	{
		GLTexture *texture = new GLTexture(path);
		textures[path] = texture;
		return texture;
	}

	void GLContext :: setDisplayList(const std::string &key, unsigned int val)
	{
		displayLists[key] = val;
	}

	void GLContext :: setVertexBuffer(const std::string &key, unsigned int val)
	{
		vertexBuffers[key] = val;
	}
}
