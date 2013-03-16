/**
 * Class:	GLFontManager
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */
#pragma once
#include <map>
#include "JDHUtility/Singleton.h"

namespace JDHUtility
{
	class GLFontRenderer;
	class Vector2f;

	class GLFontManager :
		public Singleton
	{
	public:
		static GLFontManager *getInstance(void);
		
		void renderString(std::string font, std::string s, const Vector2f &box, float border = 0.0f);

	private:
		~GLFontManager(void);

		std::map<std::string, GLFontRenderer *> renderers;
	};
}

