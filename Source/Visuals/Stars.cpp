/**
 * Class:	Stars
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */
#include <assert.h>
#include <sstream>
#include "JDHUtility/GLContext.h"
#include "JDHUtility/GlutInclude.h"
#include "JDHUtility/GLUtils.h"
#include "Particle.h"
#include "Visuals/Stars.h"

using namespace JDHUtility;

namespace WavesVisuals
{
	/* PRIVATE CONSTANTS */
	const std::string	Stars::JITTER_NAME					= "Jitter";
	const float			Stars::MAX_JITTER					= 0.05f;
	const float			Stars::MAX_Z_TRANSLATE				= 0.01f;
	const std::string	Stars::PARTICLE_DISPLAY_LIST_KEY	= "Stars_Particle";
	const std::string	Stars::SPEED_NAME					= "Speed";
	const float			Stars::STAR_DIM						= 0.01f;
	const std::string	Stars::STAR_TEXTURE_PATH			= "../Images/VisualTextures/star.tga";
	const std::string	Stars::STAR_TRAIL_TEXTURE_PATH		= "../Images/VisualTextures/startrail.tga";
	const std::string	Stars::TRAIL_DISPLAY_LIST_KEY		= "Stars_Trail";
	const std::string	Stars::TRAIL_LENGTH_NAME			= "Trail length";
	const float			Stars::Z_START_POS					= -1.0f;

	/* CONSTRUCTORS */
	Stars::Stars(void) :
		ParticleSystemVisual("Stars")
	{
		trailDirty = true;

		// display list keys
		std::stringstream ss0;
		ss0 << PARTICLE_DISPLAY_LIST_KEY << rand();
		particleDisplayListKey = ss0.str();
		
		std::stringstream ss1;
		ss1 << PARTICLE_DISPLAY_LIST_KEY << rand();
		trailDisplayListKey = ss1.str();

		// disable un-used parameters
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

		// new parameters
		parameters[SPEED_NAME] = new Parameter(SPEED_NAME);
		parameters[JITTER_NAME] = new Parameter(JITTER_NAME, 0.0f);
		parameters[TRAIL_LENGTH_NAME] = new Parameter(TRAIL_LENGTH_NAME, 0.25f);
	}

	Stars::~Stars(void)
	{
	}

	/* PUBLIC MEMBER FUNCTIONS */
	Visual *Stars::getInstance(void) const
	{
		return new Stars();
	}

	/* PRIVATE MEMBER FUNCTIONS */
	void Stars::renderParticle(Particle *p)
	{
		assert(p != NULL);

		Point3f &pos = p->getPosition();

		float x = pos.getX();
		float y = pos.getY();
		float z = pos.getZ();

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glTranslatef(x, y, z);

		glEnable(GL_TEXTURE_2D);
		GLContext *currContext = GLContext::getCurrentContext();
		assert(currContext != NULL);
		currContext->bindTexture(STAR_TEXTURE_PATH);

		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		float opacity = getOpacity();

		// push blend function
		glPushAttrib(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_ENABLE_BIT);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_ALPHA_TEST);

		glAlphaFunc(GL_GREATER, opacity / 2.0f);

		glDepthMask(true);

		glColor4f(1.0f, 1.0f, 1.0f, opacity);

		//GLUtils::billboardBegin();

		int dl = currContext->getDisplayList(particleDisplayListKey);
		if(dl == -1)
		{
			dl = glGenLists(1);
			currContext->setDisplayList(particleDisplayListKey, dl);
			glNewList(dl, GL_COMPILE);

			float dim = STAR_DIM / 2.0f;
			glBegin(GL_QUADS);
				glTexCoord2f(0.0f, 0.0f);	glVertex3f(-dim,	-dim,	0.0f);
				glTexCoord2f(1.0f, 0.0f);	glVertex3f(dim,		-dim,	0.0f);
				glTexCoord2f(1.0f, 1.0f);	glVertex3f(dim,		dim,	0.0f);
				glTexCoord2f(0.0f, 1.0f);	glVertex3f(-dim,	dim,	0.0f);
			glEnd();

			glEndList();
		}
		glCallList(dl);

		//GLUtils::billboardEnd();

		glDepthMask(false);
		glPopAttrib();
		glPopMatrix();

		glDisable(GL_TEXTURE_2D);
	}

	void Stars::renderTrail(Particle *p)
	{
		glEnable(GL_TEXTURE_2D);
		GLContext *currContext = GLContext::getCurrentContext();
		assert(currContext != NULL);
		currContext->bindTexture(STAR_TRAIL_TEXTURE_PATH);

		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// push blend function
		glPushAttrib(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);		
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);

		unsigned int trailLength = 0;
		unsigned int trailPtr = 0;
		Particle::TrailPoint **trail = p->getTrail(trailLength, trailPtr);
		assert(trail != NULL);
		
		float trailDim = STAR_DIM / 6.0f;
		glColor4f(1.0f, 1.0f, 1.0f, getOpacity());
		
		//GLUtils::billboardBegin();
		Parameter *trailLenParam = parameters[TRAIL_LENGTH_NAME];
		assert(trailLenParam != NULL);

		float trailLenf = trailLenParam->getValue();
		unsigned int trailRenderLength = (unsigned int)(trailLenf * (float)trailLength);
		unsigned int startIndex = (trailLength - trailRenderLength);


		int dl = currContext->getDisplayList(trailDisplayListKey);
		if(dl == -1)
		{
			dl = glGenLists(1);
			currContext->setDisplayList(trailDisplayListKey, dl);
		}

		if(trailDirty)
		{
			glNewList(dl, GL_COMPILE);

			glBegin(GL_QUAD_STRIP);
				for(unsigned int i = startIndex; i < trailLength; i++)
				{
					unsigned int index = (trailPtr + i) % trailLength;
					Particle::TrailPoint *trailPoint = trail[index];
					assert(trailPoint != NULL);

					if(trailPoint->enabled)
					{
						Point3f *pos = trailPoint->pos;
						assert(pos != NULL);

						float tx = pos->getX();
						float ty = pos->getY();
						float tz = pos->getZ();

						float u = (float)(i - startIndex) / (float)trailRenderLength;
						glTexCoord2f(u, 0.0f);	glVertex3f(tx, ty - trailDim, tz);
						glTexCoord2f(u, 1.0f);	glVertex3f(tx, ty + trailDim, tz);
					}
				}
			glEnd();

			glEndList();
			trailDirty = true;
		}
		glCallList(dl);

		//GLUtils::billboardEnd();
	
		glPopAttrib();
		glDisable(GL_TEXTURE_2D);
	}

	void Stars::resetParticle(Particle *p)
	{
		assert(p != NULL);

		p->setEnabled(true);
		p->resetTrail();

		// set initial position
		float px = getRandom() - 0.5f;
		float py = getRandom() - 0.5f;

		// scale to get more in the centre
		px *= 0.75f;
		py *= 0.75f;

		p->setPosition(Point3f(px, py, Z_START_POS));
	}

	void Stars::updateParticle(Particle *p)
	{
		assert(p != NULL);

		Point3f &pos = p->getPosition();

		Parameter *speedParam = parameters[SPEED_NAME];
		assert(speedParam != NULL);
		float speed = speedParam->getValue();

		// jitter
		Parameter *jitterParam = parameters[JITTER_NAME];
		assert(jitterParam != NULL);
		float xJitter = jitterParam->getValue();
		xJitter *= (getRandom() - 0.5f) * MAX_JITTER;

		float yJitter = jitterParam->getValue();
		yJitter *= (getRandom() - 0.5f) * MAX_JITTER;

		// twist
		Parameter *twistParam = parameters[TWIST_NAME];
		assert(twistParam != NULL);
		float twist = twistParam->getValue();

		float pTwist = p->getTwist();
		float theta = pTwist * twist;
		float tx = cos(theta) * pos.getX() - sin(theta) * pos.getY();
		float ty = sin(theta) * pos.getX() + cos(theta) * pos.getY(); 

		pos.setX(tx + xJitter);
		pos.setY(ty + yJitter);
		pos.translateZ(MAX_Z_TRANSLATE * speed);

		trailDirty = true;
	}
}
