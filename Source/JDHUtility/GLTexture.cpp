/**
 * Class:	GLTexture
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */
#include <exception>
#include "JDHUtility/GLTexture.h"
#include "JDHUtility/LoadImage.h"

namespace JDHUtility
{
	/* Private Constants */
	const unsigned int GLTexture::INVALID_TEXTURE = -1;

	/* Constructors */
	GLTexture::GLTexture(std::string path)
	{
		glPushAttrib(GL_ENABLE_BIT);
		
		id = createTexture(path.c_str());
		
		glPopAttrib();
	}

	GLTexture::~GLTexture(void)
	{
	}

	/* Public Member Functions */
	void GLTexture::bind(GLenum mode, GLenum minFilter, GLenum magFilter, GLenum wrapS, GLenum wrapT) const
	{
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, id);

		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT);
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, mode);
	}

	unsigned int GLTexture::getId(void) const
	{
		return id;
	}

	bool GLTexture::isTexture(void) const
	{
		return id != INVALID_TEXTURE;
	}
}
