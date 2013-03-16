/**
 * Class:	MotionPath
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */
#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include "JDHUtility/Interpolator.h"
#include "MotionPath.h"
#include "JDHUtility/Point3f.h"

namespace WavesVisuals
{
	/* PRIVATE CONSTANTS */
	const float MotionPath::DEFAULT_DISTANCE_BETWEEN_POINTS = 0.1f;
	const float MotionPath::DEFAULT_X_MAX					= 0.25f;
	const float MotionPath::DEFAULT_Y_MAX					= 0.25f;
	const float MotionPath::DEFAULT_Z_MAX					= 0.25f;

	/* CONSTRUCTORS */
	MotionPath::MotionPath(unsigned int numPoints, float xMax, float yMax, float zMax, float distanceBetweenPoints)
	{
		this->numPoints				= numPoints;
		this->xMax					= xMax;
		this->yMax					= yMax;
		this->zMax					= zMax;
		this->distanceBetweenPoints = distanceBetweenPoints;

		currentValue	= new Point3f();
		points			= new Point3f[numPoints];
		position		= 0.0f;

		initPath();
	}

	MotionPath::~MotionPath(void)
	{
		if(points != NULL)
		{
			delete [] points;
			points = NULL;
		}
	}

	/* PUBLIC MEMBER FUNCTIONS */
	const Point3f &MotionPath::getCurrentValue(void) const
	{
		return *currentValue;
	}

	unsigned int MotionPath::getNumPoints(void) const
	{
		return numPoints;
	}

	const Point3f &MotionPath::getPoint(unsigned int index) const
	{
		return points[index % numPoints];
	}

	void MotionPath::updatePosition(float increment)
	{
		assert(currentValue != NULL);

		position += increment;
		if(position > 1.0f)
		{
			position = 0.0f;
		}
		else if(position < 0.0f)
		{
			position = 1.0f;
		}

		unsigned int index	= (unsigned int)(position * (float)numPoints);
		float step			= 1.0f / (float)numPoints;
		float remainder		= position - ((float)index * step); 
		float mu			= remainder / step;
		
		const Point3f *p0 = &points[(index - 1) % numPoints];
		assert(p0 != NULL);

		const Point3f *p1 = &points[index % numPoints];
		assert(p1 != NULL);

		const Point3f *p2 = &points[(index + 1) % numPoints];
		assert(p2 != NULL);

		const Point3f *p3 = &points[(index + 2) % numPoints];
		assert(p3 != NULL);

		float cx = Interpolator::hermiteInterpolate(p0->getX(), p1->getX(), p2->getX(), p3->getX(), mu, Interpolator::DEFAULT_TENSION, 0.0f);
		float cy = Interpolator::hermiteInterpolate(p0->getY(), p1->getY(), p2->getY(), p3->getY(), mu, Interpolator::DEFAULT_TENSION, 0.0f);
		float cz = Interpolator::hermiteInterpolate(p0->getZ(), p1->getZ(), p2->getZ(), p3->getZ(), mu, Interpolator::DEFAULT_TENSION, 0.0f);
		
		currentValue->setX(cx);
		currentValue->setY(cy);
		currentValue->setZ(cz);
	}

	/* PRIVATE MEMBER FUNCTIONS */
	void MotionPath::initPath(void)
	{
		float x = 0.0f;
		float y = 0.0f;
		float z = 0.0f;

		// init points
		for(unsigned int i = 0; i < numPoints; i++)
		{
			// set as pos
			Point3f *p = &points[i];
			assert(p != NULL);

			p->setX(x);
			p->setY(y);
			p->setZ(z);

			// generate random normalised vector
			float vx		= ((float)rand() / (float)RAND_MAX) - 0.5f;
			float vy		= ((float)rand() / (float)RAND_MAX) - 0.5f;
			//float vz		= 0.0f;//((float)rand() / (float)RAND_MAX) - 0.5f;
			float magnitude	= sqrt((vx * vx) + (vy * vy));// + (vz * vz));

			vx = vx * (1.0f / magnitude);
			vy = vy * (1.0f / magnitude);
			//vz = vz * (1.0f / magnitude);

			x += (vx * distanceBetweenPoints);
			y += (vy * distanceBetweenPoints);
			//z += (vz * distanceBetweenPoints);
			z = 0.0f;

			// clamp
			if(x > xMax / 2.0f) x = xMax / 2.0f;
			else if(x < -xMax / 2.0f) x = -xMax / 2.0f;

			if(y > yMax / 2.0f) y = yMax / 2.0f;
			else if(y < -yMax / 2.0f) y = -yMax / 2.0f;

			//if(z > zMax / 2.0f) z = zMax / 2.0f;
			//else if(z < -zMax / 2.0f) z = -zMax / 2.0f;
		}
	}
}
