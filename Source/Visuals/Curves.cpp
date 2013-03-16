/**
 * Class:	Curves
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */
#define _USE_MATH_DEFINES
#include <assert.h>
#include <math.h>
#include "Visuals/Curves.h"
#include "JDHUtility/GlutInclude.h"
#include "MotionPath.h"

namespace WavesVisuals
{
	/* PRIVATE CONSTANTS */
	const std::string	Curves::LINE_LENGTH_NAME			= "Line Length";
	const std::string	Curves::LINE_SPEED_NAME				= "Line Speed";
	const float			Curves::MAX_LINE_LENGTH				= 0.75f;
	const float			Curves::MAX_OPACITY					= 0.75f;
	const float			Curves::MAX_MOTION_PATH_INCREMENT	= 0.0005f;

	/* CONSTRUCTORS */
	Curves::Curves(unsigned int maxNumLines) :
		Visual("Curves")
	{
		this->maxNumLines = maxNumLines;
		
		firstLine	= true;
		linePtr		= 0;
		motionPath	= new MotionPath();
		sinVal		= 0.0f;

		lines = new LineProfile[maxNumLines];
		for(unsigned int i = 0; i < maxNumLines; i++)
		{
			LineProfile *lp = &lines[i];
			assert(lp != NULL);

			lp->enabled = false;
		}

		// new parameters
		parameters[LINE_LENGTH_NAME]	= new Parameter(LINE_LENGTH_NAME, 0.5f);
		parameters[LINE_SPEED_NAME]		= new Parameter(LINE_SPEED_NAME);

		// disable parameters
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
	}

	Curves::~Curves(void)
	{
		if(motionPath != NULL)
		{
			delete motionPath;
			motionPath = NULL;
		}
	}

	/* PUBLIC MEMBER FUNCTIONS */
	void Curves::draw(void)
	{
		glEnable(GL_LINE_SMOOTH);

		float opacity = getOpacity() * MAX_OPACITY;
		for(unsigned int i = 0; i < maxNumLines; i++)
		{
			LineProfile *lp = &lines[i];
			assert(lp != NULL);

			if(lp->enabled)
			{
				glMatrixMode(GL_MODELVIEW);
				glPushMatrix();
				glTranslatef(lp->centre.getX(), lp->centre.getY(), lp->centre.getZ());
				
				
				glRotatef(lp->X * 360.0f, 1.0f, 0.0f, 0.0f);
				glRotatef(lp->Y * 360.0f, 0.0f, 1.0f, 0.0f);

				glBegin(GL_LINE_STRIP);
					glColor4f(1.0f, 1.0f, 1.0f, 0.0f);
					glVertex3f(-lp->length,			0.0f, 0.0f);
					
					glColor4f(1.0f, 1.0f, 1.0f, opacity);
					glVertex3f(lp->length * 0.5f,	0.0f, 0.0f);
					
					glColor4f(1.0f, 1.0f, 1.0f, 0.0f);
					glVertex3f(lp->length,			0.0f, 0.0f);
				glEnd();

				glPopMatrix();
			}
		}

		glDisable(GL_LINE_SMOOTH);
	}
	
	Visual *Curves::getInstance(void) const
	{
		return new Curves();	
	}
	
	void Curves::update(void)
	{
		assert(motionPath != NULL);

		unsigned int now = glutGet(GLUT_ELAPSED_TIME);
		unsigned int elapsed = now - last;

		if(elapsed >= UPDATE_RATE_MS)
		{
			Parameter *speedParam = parameters[LINE_SPEED_NAME];
			assert(speedParam != NULL);
			float speed = speedParam->getValue();

			motionPath->updatePosition(speed * MAX_MOTION_PATH_INCREMENT);
			addLine();

			last = now;
		}
	}

	/* PRIVATE MEMBER FUNCTIONS */
	void Curves::addLine(void)
	{
		assert(motionPath != NULL);

		const Point3f &p = motionPath->getCurrentValue();
		
		Parameter *lenParam = parameters[LINE_LENGTH_NAME];
		assert(lenParam != NULL);
		float lineLen = lenParam->getValue() * MAX_LINE_LENGTH;

		LineProfile *l	= &lines[linePtr];
		assert(l != NULL);

		Point3f *centre = &l->centre;
		assert(centre != NULL);

		l->enabled		= true;
		l->length		= lineLen;
		
		centre->setX(p.getX());
		centre->setY(p.getY());
		centre->setZ(p.getZ());

		if(firstLine)
		{
			l->X = 0.0f;
			l->Y = 0.0f;
			l->Z = 0.0f;
			firstLine = false;
		}
		else
		{
			LineProfile *last = &lines[(linePtr - 1) % maxNumLines];
			assert(last != NULL);

			Point3f *lastCentre = &last->centre;
			assert(lastCentre != NULL);

			// get normalised vector between now and last
			float x0 = p.getX();
			float y0 = p.getY();
			float z0 = p.getZ();
			float x1 = lastCentre->getX();
			float y1 = lastCentre->getY();
			float z1 = lastCentre->getZ();
			float vx = x1 - x0;
			float vy = y1 - y0;
			float vz = z1 - z0;
			float magnitude	= sqrt((vx * vx) + (vy * vy) + (vz * vz));
			vx = vx * (1.0f / magnitude);
			vy = vy * (1.0f / magnitude);
			vz = vz * (1.0f / magnitude);

			l->X = vx;
			l->Y = vy;
			l->Z = vz;
		}

		linePtr = (linePtr + 1) % maxNumLines;
	}

	void Curves::rotatePoint(Point3f &p, float ax, float ay, float az) const
	{
		float x0 = p.getX();
		float y0 = p.getY();
		float z0 = p.getZ();

		// X ROTATE
		y0 = y0 * cos(ax) - z0 * sin(ax);
		z0 = y0 * cos(ax) + z0 * cos(ax);

		// Y ROTATE
		z0 = z0 * cos(ay) - x0 * sin(ay);
		x0 = z0 * sin(ay) + x0 * cos(ay);

		// Z ROTATE
		x0 = x0 * cos(az) - y0 * sin(az);
		y0 = x0 * sin(az) + y0 * cos(az);

		p.setX(x0);
		p.setX(y0);
		p.setX(z0);
	}
}
