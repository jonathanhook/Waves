/**
 * Class:	GLFontRenderer
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */
#ifndef FontRenderer_H
#define FontRenderer_H

#include <string>
#include <windows.h>
#include "JDHUtility/Point2f.h"
#include "JDHUtility/Vector2f.h"

namespace JDHUtility
{
	class GLFontRenderer
	{
	public:
		GLFontRenderer(std::string fontName);
		~GLFontRenderer(void);

		void renderString(const std::string &s, const Vector2f &box, float border = 0.0f) const;

	private:
		static const float CHAR_HEIGHT;
		static const float CHAR_WIDTH;
		static const std::string DEFAULT_FONT;
		static const char FIRST_CHAR;
		static const int NUM_CHARS;

		int pbase;
		int lbase;
		int bbase;
		HDC hDC;
	};
}
#endif
