/**
 * Class:	Visual
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */
#define _USE_MATH_DEFINES
#include <assert.h>
#include <exception>
#include <math.h>
#include <stdlib.h>
#include "JDHUtility/ColourConvertor.h"
#include "JDHUtility/GlutInclude.h"
#include "Visuals/Visual.h"
#include "Visuals/Parameter.h"

namespace WavesVisuals
{
	/* PROTECTED CONSTANTS */
	const std::string	Visual::COMBINED_SCALE_NAME		= "Scale";
	const float			Visual::COMBINED_SCALE_DEFAULT	= 0.5f;
	const float			Visual::GLOBAL_OPACITY_DEFAULT	= 1.0f;
	const std::string	Visual::GLOBAL_OPACITY_NAME		= "Global Opacity";
	const std::string	Visual::HUE_NAME				= "Hue";
	const float			Visual::HUE_DEFAULT				= 0.5f;
	const std::string	Visual::LUMINANCE_NAME			= "Luminance";
	const float			Visual::LUMINANCE_DEFAULT		= 0.5f;
	const std::string	Visual::OPACITY_NAME			= "Opacity";
	const float			Visual::OPACITY_DEFAULT			= 0.5f;
	const float			Visual::POSITION_DEFAULT		= 0.5f;
	const std::string	Visual::POSITION_X_NAME			= "Position X";
	const std::string	Visual::POSITION_Y_NAME			= "Position Y";
	const std::string	Visual::POSITION_Z_NAME			= "Position Z";
	const std::string	Visual::ROTATION_X_NAME			= "Rotation X";
	const std::string	Visual::ROTATION_Y_NAME			= "Rotation Y";
	const std::string	Visual::ROTATION_Z_NAME			= "Rotation Z";
	const float			Visual::ROTATION_DEFAULT		= 0.5f;
	const std::string	Visual::SATURATION_NAME			= "Saturation";
	const float			Visual::SATURATION_DEFAULT		= 0.5f;
	const std::string	Visual::SCALE_X_NAME			= "Scale X";
	const std::string	Visual::SCALE_Y_NAME			= "Scale Y";
	const std::string	Visual::SCALE_Z_NAME			= "Scale Z";
	const float			Visual::SCALE_DEFAULT			= 0.5f;

	/* CONSTRUCTORS */
	Visual::Visual(std::string name, bool combinedScale)
	{
		this->name = name;

		initParameters(combinedScale);

		depthBuffer = NULL;
		frameBufferObject = NULL;
		thumbnailRendered = false;

		glGenTextures(1, &texture);	
	}

	Visual::~Visual(void)
	{
	}

	/* PUBLIC MEMBER FUNCTIONS */
	Colour3f Visual::getColour(void)
	{
		Parameter *h = parameters[HUE_NAME];
		Parameter *s = parameters[SATURATION_NAME];
		Parameter *l = parameters[LUMINANCE_NAME];

		assert(h != NULL);
		assert(s != NULL);
		assert(l != NULL);

		Vector3f hsl;
		hsl.setX(h->getValue());
		hsl.setY(s->getValue());
		hsl.setZ(l->getValue());

		return ColourConvertor::hslToRgb(hsl);
	}

	const std::string &Visual::getName(void) const
	{
		return name;
	}

	float Visual::getOpacity(void)
	{
		Parameter *opacityP = parameters[OPACITY_NAME];
		assert(opacityP != NULL);
		float opacity = opacityP->getValue();

		Parameter *globalOpacityP = parameters[GLOBAL_OPACITY_NAME];
		assert(globalOpacityP != NULL);
		float globalOpacity = globalOpacityP->getValue();

		return opacity * globalOpacity;
	}

	Parameter *Visual::getParameter(std::string key)
	{
		if(parameters.count(key) > 0)
		{
			return parameters[key];
		}
		else return NULL;
	}

	const std::map<std::string, Parameter *> Visual::getParameters(void) const
	{
		return parameters;
	}

	Point3f Visual::getPosition(void)
	{
		Parameter *px = parameters[POSITION_X_NAME];
		Parameter *py = parameters[POSITION_Y_NAME];
		Parameter *pz = parameters[POSITION_Z_NAME];

		assert(px != NULL);
		assert(py != NULL);
		assert(pz != NULL);

		return Point3f(px->getValue(), py->getValue(), pz->getValue());
	}

	Vector3f Visual::getRotation(void)
	{
		Parameter *rx = parameters[ROTATION_X_NAME];
		Parameter *ry = parameters[ROTATION_Y_NAME];
		Parameter *rz = parameters[ROTATION_Z_NAME];

		assert(rx != NULL);
		assert(ry != NULL);
		assert(rz != NULL);

		return Vector3f(rx->getValue(), ry->getValue(), rz->getValue());
	}

	Vector3f Visual::getScale(void)
	{
		if(parameters[COMBINED_SCALE_NAME]->isDisabled())
		{
			Parameter *sx = parameters[SCALE_X_NAME];
			Parameter *sy = parameters[SCALE_Y_NAME];
			Parameter *sz = parameters[SCALE_Z_NAME];
			
			assert(sx != NULL);
			assert(sy != NULL);
			assert(sz != NULL);

			return Vector3f(sx->getValue(), sy->getValue(), sz->getValue());
		}
		else
		{
			Parameter *cs =  parameters[COMBINED_SCALE_NAME];
			assert(cs != NULL);

			return Vector3f(cs->getValue(), cs->getValue(), cs->getValue());
		}		
	}

	void Visual::render(void)
	{
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
			
		glTranslatef(-POSITION_DEFAULT + getPosition().getX(), -POSITION_DEFAULT + getPosition().getY(), -POSITION_DEFAULT + getPosition().getZ());
		glRotatef(getRotation().getX() * 360.0f, 1.0f, 0.0f, 0.0f);
		glRotatef(getRotation().getY() * 360.0f, 0.0f, 1.0f, 0.0f);
		glRotatef(getRotation().getZ() * 360.0f, 0.0f, 0.0f, 1.0f);
		glScalef(getScale().getX() / SCALE_DEFAULT, getScale().getY() / SCALE_DEFAULT, getScale().getZ() / SCALE_DEFAULT);
		
		const Colour3f &c = getColour();
		glColor4f(c.getR(), c.getG(), c.getB(), getOpacity());

		draw();

		glPopMatrix();		
	}

	unsigned int Visual::renderThumbnail(bool renderWithDefaultValues)
	{
		if(!thumbnailRendered)
		{
			if(depthBuffer == NULL || frameBufferObject == NULL || texture == NULL)
			{
				glGenFramebuffersEXT(1, &frameBufferObject);
				glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, frameBufferObject);
				glGenRenderbuffersEXT(1, &depthBuffer);
				glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, depthBuffer);
				glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT, TEXTURE_WIDTH, TEXTURE_HEIGHT);
				glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, depthBuffer);

				glGenTextures(1, &texture);
				glBindTexture(GL_TEXTURE_2D, texture);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, TEXTURE_WIDTH, TEXTURE_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		
				// set textures not to use mipmaps
				glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				//glGenerateMipmapEXT(GL_TEXTURE_2D);
		
				glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, texture, 0);
			}

			// check all is ok with our FBO before beginning to render
			GLenum status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
			if(status != GL_FRAMEBUFFER_COMPLETE_EXT) 
			{
	#			ifdef _DEBUG
				static bool errorDisplayed = false;
				if(!errorDisplayed)
				{
					printf("Error: frame buffer not initialised fully\n");
					errorDisplayed = true;
				}
	#			endif
				printf("%i\n", glGetError());
				return -1;
			}
			else
			{
				// render
				glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, frameBufferObject);
		
				glPushAttrib(GL_ALL_ATTRIB_BITS);
				glViewport(0, 0, TEXTURE_WIDTH, TEXTURE_HEIGHT);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				glEnable(GL_BLEND); 
				glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				glEnable(GL_LINE_SMOOTH);
			
				glMatrixMode(GL_MODELVIEW);
				glPushMatrix();
				glLoadIdentity();


				// scale to account for camera position
				float ratio = (float)glutGet(GLUT_WINDOW_WIDTH) / (float)glutGet(GLUT_WINDOW_HEIGHT);
				glTranslatef(0.5f, 0.5f / ratio, 0.0f);
				glScalef(0.5f, 0.5f, 0.5f);

				if(renderWithDefaultValues)
				{
					for(std::map<std::string, Parameter *>::iterator it = parameters.begin(); it != parameters.end(); it++)
					{
						Parameter *p = (*it).second;
						assert(p != NULL);

						if(p->getName() == "Opacity")
						{
							p->setValue(1.0f);
						}
						else
						{
							p->setValue(p->getDefaultValue());
						}
					}
				}

				render();
		
				glPopMatrix();
				glPopAttrib();
			
				glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
			}

			thumbnailRendered = true;
		}

		return texture;
	}

	void Visual::setColour(const Colour3f &colour)
	{
		// TODO: colour conversion
		/*parameters[HUE_NAME]->setValue(1.0f);
		parameters[SATURATION_NAME]->setValue(1.0f);
		parameters[LUMINANCE_NAME]->setValue(1.0f);*/
	}

	void Visual::setOpacity(float opacity)
	{
		Parameter *opacityParam = parameters[OPACITY_NAME];
		assert(opacityParam != NULL);

		opacityParam->setValue(opacity); 
	}

	void Visual::setParameterToDefault(std::string key)
	{
		if(parameters.count(key) > 0)
		{
			Parameter *p = parameters[key];
			assert(p != NULL);

			p->setValue(parameters[key]->getDefaultValue());
		}
	}

	void Visual::setParameterValue(std::string key, float value)
	{
		if(parameters.count(key) > 0)
		{
			Parameter *p = parameters[key];
			assert(p != NULL);

			p->setValue(value);
		}
	}

	void Visual::setPosition(const Point3f &position)
	{
		Parameter *px = parameters[POSITION_X_NAME];
		Parameter *py = parameters[POSITION_Y_NAME];
		Parameter *pz = parameters[POSITION_Z_NAME];

		assert(px != NULL);
		assert(py != NULL);
		assert(pz != NULL);

		px->setValue(position.getX());
		py->setValue(position.getY());
		pz->setValue(position.getZ());
	}

	void Visual::setRotation(const Vector3f &rotation)
	{
		Parameter *rx = parameters[ROTATION_X_NAME];
		Parameter *ry = parameters[ROTATION_Y_NAME];
		Parameter *rz = parameters[ROTATION_Z_NAME];

		assert(rx != NULL);
		assert(ry != NULL);
		assert(rz != NULL);

		rx->setValue(rotation.getX());
		ry->setValue(rotation.getY());
		rz->setValue(rotation.getZ());
	}

	void Visual::setScale(const Vector3f &scale)
	{
		Parameter *sx = parameters[SCALE_X_NAME];
		Parameter *sy = parameters[SCALE_Y_NAME];
		Parameter *sz = parameters[SCALE_Z_NAME];

		assert(sx != NULL);
		assert(sy != NULL);
		assert(sz != NULL);

		sx->setValue(scale.getX());
		sy->setValue(scale.getY());
		sz->setValue(scale.getZ());
	}

	/* PROTECTED MEMBER FUNCTIONS */
	float Visual::getRandom(void) const
	{
		return (float)rand() / (float)RAND_MAX;
	}

	/* PRIVATE MEMBER FUNCTIONS */
	void Visual::initParameters(bool combinedScale)
	{
			parameters[COMBINED_SCALE_NAME] = new Parameter(COMBINED_SCALE_NAME, COMBINED_SCALE_DEFAULT);
			parameters[GLOBAL_OPACITY_NAME] = new Parameter(GLOBAL_OPACITY_NAME, GLOBAL_OPACITY_DEFAULT);
			parameters[HUE_NAME] = new Parameter(HUE_NAME, HUE_DEFAULT);
			parameters[LUMINANCE_NAME] = new Parameter(LUMINANCE_NAME, LUMINANCE_DEFAULT);
			parameters[OPACITY_NAME] = new Parameter(OPACITY_NAME, OPACITY_DEFAULT);
			parameters[POSITION_X_NAME] = new Parameter(POSITION_X_NAME, POSITION_DEFAULT);
			parameters[POSITION_Y_NAME] = new Parameter(POSITION_Y_NAME, POSITION_DEFAULT);
			parameters[POSITION_Z_NAME] = new Parameter(POSITION_Z_NAME, POSITION_DEFAULT);
			parameters[ROTATION_X_NAME] = new Parameter(ROTATION_X_NAME, ROTATION_DEFAULT);
			parameters[ROTATION_Y_NAME] = new Parameter(ROTATION_Y_NAME, ROTATION_DEFAULT);
			parameters[ROTATION_Z_NAME] = new Parameter(ROTATION_Z_NAME, ROTATION_DEFAULT);
			parameters[SATURATION_NAME] = new Parameter(SATURATION_NAME, SATURATION_DEFAULT);
			parameters[SCALE_X_NAME] = new Parameter(SCALE_X_NAME, SCALE_DEFAULT);
			parameters[SCALE_Y_NAME] = new Parameter(SCALE_Y_NAME, SCALE_DEFAULT);
			parameters[SCALE_Z_NAME] = new Parameter(SCALE_Z_NAME, SCALE_DEFAULT);
	
			if(combinedScale)
			{
				parameters[SCALE_X_NAME]->setDisabled(true);
				parameters[SCALE_Y_NAME]->setDisabled(true);
				parameters[SCALE_Z_NAME]->setDisabled(true);
				parameters[COMBINED_SCALE_NAME]->setDisabled(false);
			}
			else
			{
				parameters[COMBINED_SCALE_NAME]->setDisabled(true);
			}
	}
}
