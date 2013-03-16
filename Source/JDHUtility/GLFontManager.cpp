/**
 * Class:	GLFontManager
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */
#include <assert.h>
#include "JDHUtility/GLFontRenderer.h"
#include "GLFontManager.h"
#include "Ndelete.h"

namespace JDHUtility
{
	/* Private Constructors */
	GLFontManager::~GLFontManager(void)
	{
		std::map<std::string, GLFontRenderer *>::iterator it;
		for(it = renderers.begin(); it != renderers.end(); it++)
		{
			NDELETE((*it).second);
		}
	}

	/* Public Static Functions */
	GLFontManager *GLFontManager::getInstance(void)
	{
		if(instance == NULL)
		{
			instance = new GLFontManager();
		}

		return (GLFontManager *)instance;
	}

	/* Public Member Functions */
	void GLFontManager::renderString(std::string font, std::string s, const Vector2f &box, float border)
	{
		if(renderers.count(font) == 0)
		{
			GLFontRenderer *r = new GLFontRenderer(font);
			renderers[font] = r;
		}

		GLFontRenderer *renderer = renderers[font];
		assert(renderer);

		renderer->renderString(s, box, border);
	}
}
