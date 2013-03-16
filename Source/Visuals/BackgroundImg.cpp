/**
 * Class:	BackgroundImg
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */
#include <math.h>
#include <sstream>
#include "Visuals/BackgroundImg.h"
#include "JDHUtility/GlutInclude.h"
#include "JDHUtility/GLContext.h"
#include "Waves/InterfaceManager.h"

using namespace Waves;

namespace WavesVisuals
{
	/* PRIVATE CONSTANTS */
	const std::string	BackgroundImg::DISPLAY_LIST_KEY	= "BackgroundImg";
	const float			BackgroundImg::Z_POS			= -5.0f;

	/* CONSTRUCTORS */
	BackgroundImg::BackgroundImg(std::string path, std::string name) :
		Visual(name)
	{
		this->path = path;

		// display list key
		stringstream ss;
		ss << DISPLAY_LIST_KEY << rand();
		displayListKey = ss.str();

		Parameter *hueP = parameters[HUE_NAME];
		assert(hueP != NULL);
		hueP->setDisabled(true);
		
		Parameter *lumaP = parameters[LUMINANCE_NAME];
		assert(lumaP != NULL);
		lumaP->setDisabled(true);

		Parameter *saturationP = parameters[SATURATION_NAME];
		assert(saturationP != NULL);
		saturationP->setDisabled(true);

		Parameter *rotXP = parameters[ROTATION_X_NAME];
		assert(rotXP != NULL);
		rotXP->setDisabled(true);

		Parameter *rotYP = parameters[ROTATION_Y_NAME];
		assert(rotYP != NULL);
		rotYP->setDisabled(true);

		Parameter *rotZP = parameters[ROTATION_Z_NAME];
		assert(rotZP != NULL);
		rotZP->setDisabled(true);

		Parameter *posXP = parameters[POSITION_X_NAME];
		assert(posXP != NULL);
		posXP->setDisabled(true);

		Parameter *posYP = parameters[POSITION_Y_NAME];
		assert(posYP != NULL);
		posYP->setDisabled(true);

		Parameter *posZP = parameters[POSITION_Z_NAME];
		assert(posZP != NULL);
		posZP->setDisabled(true);

		Parameter *comsP = parameters[COMBINED_SCALE_NAME];
		assert(comsP != NULL);
		comsP->setDisabled(true);
	}

	BackgroundImg::~BackgroundImg(void)
	{
	}

	/* PUBLIC MEMBER FUNCTIONS */
	void BackgroundImg::draw(void)
	{
		glEnable(GL_TEXTURE_2D);
		GLContext *currContext = GLContext::getCurrentContext();
		assert(currContext != NULL);
		currContext->bindTexture(path);
		
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glColor4f(1.0f, 1.0f, 1.0f, getOpacity());

		int dl = currContext->getDisplayList(displayListKey);
		if(dl == -1)
		{
			dl = glGenLists(1);
			currContext->setDisplayList(displayListKey, dl);
			glNewList(dl, GL_COMPILE);

			float wWidth	= (float)glutGet(GLUT_WINDOW_WIDTH);
			float wHeight	= (float)glutGet(GLUT_WINDOW_HEIGHT);
		
			if(wHeight == 0.0f) 
			{
				wHeight = 1.0f;	
			}
		
			float ratio		= wWidth / wHeight;
			float theta		= InterfaceManager::CAMERA_FOV;
			float opposite	= abs(Z_POS) * tan(theta);
			float scale		= opposite / ratio;
			float xDim		= (ratio / 2.0f) * scale;
			float yDim		= (1.0f / 2.0f) * scale;

			glBegin(GL_QUADS);
				glTexCoord2f(0.0f, 0.0f);	glVertex3f(-xDim,	-yDim,	Z_POS);
				glTexCoord2f(1.0f, 0.0f);	glVertex3f(xDim,	-yDim,	Z_POS);
				glTexCoord2f(1.0f, 1.0f);	glVertex3f(xDim,	yDim,	Z_POS);
				glTexCoord2f(0.0f, 1.0f);	glVertex3f(-xDim,	yDim,	Z_POS);
			glEnd();

			glEndList();
		}

		glCallList(dl);

		glDisable(GL_TEXTURE_2D);
	}

	void BackgroundImg::update(void)
	{

	}
}