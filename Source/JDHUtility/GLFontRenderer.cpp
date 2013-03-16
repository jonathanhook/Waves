/**
 * Class:	GLFontRenderer
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

#include <exception>
#include <string>
#include <windows.h>
#include "JDHUtility/DebugTimer.h"
#include "JDHUtility/GLFontRenderer.h"

namespace JDHUtility
{
	/* STATIC VARS */
	const float GLFontRenderer::CHAR_HEIGHT = 0.5f;
	const float GLFontRenderer::CHAR_WIDTH = 0.53f;
	const char GLFontRenderer::FIRST_CHAR = 0;
	const int GLFontRenderer::NUM_CHARS =256;

	/* CONSTRUCTORS */
	GLFontRenderer::GLFontRenderer(std::string fontName)
	{
		hDC = wglGetCurrentDC();
		pbase = glGenLists(NUM_CHARS);	
		lbase = glGenLists(NUM_CHARS);	
		bbase = glGenLists(NUM_CHARS);

		int lfHeight = -MulDiv(8, GetDeviceCaps(hDC, LOGPIXELSY), 72);
		HFONT font = CreateFont(lfHeight,
								0, 
								0, 
								0, 
								FW_NORMAL, 
								FALSE, FALSE, FALSE,
								ANSI_CHARSET, 
								OUT_TT_PRECIS, 
								CLIP_DEFAULT_PRECIS, 
								DRAFT_QUALITY, 
								0, 
								fontName.c_str());

		SelectObject(hDC, font);
		
		GLYPHMETRICSFLOAT pgmf[256]; 
		wglUseFontOutlines(hDC,
							0,
							NUM_CHARS,
							pbase,
							1.0f,
							0,
							WGL_FONT_POLYGONS,
							pgmf);

		GLYPHMETRICSFLOAT lgmf[256]; 
		wglUseFontOutlines(hDC,
							0,
							NUM_CHARS,
							lbase,
							1.0f,
							0,
							WGL_FONT_LINES,
							lgmf);
		
		DeleteObject(font);
	}

	GLFontRenderer::~GLFontRenderer(void)
	{
		glDeleteLists(pbase, NUM_CHARS);
		glDeleteLists(lbase, NUM_CHARS);
	}

	/* PUBLIC STATIC FUNCTIONS */
	void GLFontRenderer::renderString(const std::string &s, const Vector2f &box, float border) const
	{
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();

		// flip
		glScalef(1.0f, -1.0f, 1.0f);
		float borderDim = box.getY() * border;
		glTranslatef(borderDim, -box.getY() + borderDim, 0.0f);

		float width = (box.getX() / CHAR_WIDTH) / (float)s.length();
		glScalef(width - (borderDim * 2.0f), (box.getY() - (borderDim * 2.0f)) / CHAR_HEIGHT, 1.0f);

		// render polygons
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();

		glPushAttrib(GL_LIST_BIT);
		glListBase(pbase);
		glCallLists(s.length(), GL_UNSIGNED_BYTE, s.c_str());
		glPopAttrib();

		glPopMatrix();

		// render lines
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();

		glLineWidth(1.0f);
		glPushAttrib(GL_LIST_BIT);
		glListBase(lbase);
		glCallLists(s.length(), GL_UNSIGNED_BYTE, s.c_str());
		glPopAttrib();

		glPopMatrix();
		glPopMatrix();
	}
}