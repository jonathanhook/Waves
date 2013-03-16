/**
 * Class:	GLContext
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */
#ifndef GLContext_H
#define GLContext_H

#include <map>
#include <string>
#include <vector>
#include <windows.h>
#include "JDHUtility/GLFontRenderer.h"
#include "JDHUtility/GLTexture.h"

namespace JDHUtility
{
	class GLContext
	{
	public:
		static const std::string DEFAULT_FONT;

		static GLContext *getCurrentContext(void);
		static GLContext *initialiseCurrentContext(std::string font = DEFAULT_FONT);

		void bindTexture(const std::string &path);
		int getDisplayList(const std::string &key) const;
		int getVertexBuffer(const std::string &key) const;
		void renderString(std::string s, const Vector2f &box, float border = 0.0f) const;
		void setDisplayList(const std::string &key, unsigned int val);
		void setVertexBuffer(const std::string &key, unsigned int val);
		
	private:
		static std::map<HGLRC, GLContext *> instances;
		static HGLRC lookupCurrentContext(void);

		GLContext(std::string font = DEFAULT_FONT);
		~GLContext(void);

		GLFontRenderer *fontRenderer;
		std::map<std::string, unsigned int> displayLists;
		std::map<std::string, GLTexture *> textures;
		std::map<std::string, unsigned int> vertexBuffers;

		GLTexture *loadTexture(const std::string &path);
	};
}
#endif

