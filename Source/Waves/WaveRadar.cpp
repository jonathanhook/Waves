#ifndef GLUT_INCLUDED
#define GLUT_INCLUDED
// stdio.h must be included before glut on all occasions
#include <stdio.h>
#include <stdlib.h>
	#ifdef __APPLE__
		#include <GLUT/glut.h>
	#else
		#include <GL/glut.h>
	#endif
#endif

#define _USE_MATH_DEFINES	// allows M_PI to be used
#include <math.h>
#include "UT.h"
#include "JDHUtility/Interpolator.h"
#include "JDHUtility/WrappedComparison.h"
#include "WaveCanvas.h"
#include "WaveRadar.h"

static inline float signof(float a) { return (a == 0) ? 0 : (a < 0.0f ? -1.0f : 1.0f); }

WaveRadar::WaveRadar(int beats, int upSampleFactor, WaveVisual* visual, UT::Point2f position)
{
	WaveRadar::beats = beats;
	WaveRadar::visual = visual;
	WaveRadar::upSampleFactor = upSampleFactor;
	WaveRadar::position = position;

	id = rand();

	angleOffset = 0.0f;
	hitThreshold = 0.005f;
	innerBoundary = 0.325f;
	increments = 5;
	maxDistance = M_PI / 6;
	outerBoundary = 0.95f;
	playMode = PLAYMODE_RADAR;
	radius = 0.5f;
	selectionBarWidth = (radius - (radius * 1.333f)) * 0.75f;
	offSideXPos = (radius + (radius * 1.333f)) / 2.0f;

	opacitySliderDim = (radius - (radius * 1.333f)) * 0.5f;
	opacitySliderIncrements = 5.0f;

	backgroundDL = -1;
	playheadDL = -1;
	selectionBarDL = -1;
	opacitySliderDL = -1;
}

WaveRadar::~WaveRadar()
{

}

void WaveRadar::fingerAdded(int fId, float x, float y)
{
	float a, r;
	getPolarCoordinate(x, y, a, r);
	
	string paramName;
	if(isInCentre(r))
	{
		togglePlayMode();
	}
	else if(isInSelectionBox(a, r, paramName) && paramName != "-1")
	{
		VisualParameter* param = visual->getVisualParameter(paramName);
		param->setSelected(!param->getSelected());
		dirtyFlag = true;
	}
	else if(isInOpacityBar(x, y, WaveCanvas::RENDERMODE_SINGLERADAR))
	{
		UT::Finger f;
		f.fId = fId;
		f.lastX = x;
		f.lastY = y;
		f.fingerMode = UT::FINGER_OPACITY;
		fingers[fId] = f;

		visual->setOpacity(getOpacitySliderPosition(x, y, WaveCanvas::RENDERMODE_SINGLERADAR));
		dirtyFlag = true;
	}
	else if(findClosestWaveHit(x, y, paramName) && paramName != "-1")
	{
		r = fromWaveSpace(r);
		// Update rotation according to angle offset
		a = getOffsetTouchAngle(a);

		UT::Finger f;
		f.fId = fId;
		f.lastX = x;
		f.lastY = y;
		f.parameter = paramName;
		f.lastControlA = a;
		f.lastControlR = r;
		f.fingerMode = UT::FINGER_WAVE;
		fingers[fId] = f;
	}
}

void WaveRadar::fingerUpdated(int fId, float x, float y)
{
	if(fingers.count(fId) > 0 && fingers[fId].fingerMode == UT::FINGER_WAVE)
	{
		float a0 = fingers[fId].lastControlA;
		float r0 = fingers[fId].lastControlR;

		float a1, r1;
		getPolarCoordinate(x, y, a1, r1);
		r1 = fromWaveSpace(r1);

		// Update rotation according to angle offset
		a1 = getOffsetTouchAngle(a1);

		if(r1 <= 0.0f) r1 = 0.0f;
		else if(r1 >= 1.0f) r1 = 1.0f;

		int length;
		float* values = visual->getVisualParameter(fingers[fId].parameter)->getValues(length);
		float incrementAngle = (M_PI * 2.0f) / length;
		
		for(int i = 0; i < length; i++)
		{
			float rn = getNewDistance(fId, fingers[fId].parameter, incrementAngle * (float)i, values[i], a0, a1, r0, r1);			
			visual->getVisualParameter(fingers[fId].parameter)->setValue(i, rn);
		}

		fingers[fId].lastX = x;
		fingers[fId].lastY = y;
		fingers[fId].lastControlA = a1;
		fingers[fId].lastControlR = r1;
	}
	else if(fingers.count(fId) > 0 && fingers[fId].fingerMode == UT::FINGER_OPACITY)
	{
		fingers[fId].lastX = x;
		fingers[fId].lastY = y;

		visual->setOpacity(getOpacitySliderPosition(x, y, WaveCanvas::RENDERMODE_SINGLERADAR));
		dirtyFlag = true;
	}
	else
	{
		fingerAdded(fId, x, y);
	}
}

void WaveRadar::fingerRemoved(int fId, float x, float y)
{
	if(fingers.count(fId) > 0)
	{
		fingers.erase(fId);
	}
}

int WaveRadar::getId()
{
	return id;
}

float WaveRadar::getRadius()
{
	return radius;
}

UT::Point2f WaveRadar::getPosition()
{
	return position;
}

bool WaveRadar::isInCentre(float r)
{
	return r < (radius * innerBoundary);
}

bool WaveRadar::isInSelectionBox(float a, float r, string &name)
{
	float x, y;
	getCartesianCoordinate(a, r, x, y);

	int paramCount;
	string* paramNames = visual->getParameters(paramCount);

	float xMin = -(offSideXPos - (selectionBarWidth / 2.0f));
	float xMax = -(offSideXPos + (selectionBarWidth / 2.0f));

	for(int i = 0; i < paramCount; i++)
	{
		VisualParameter* param = visual->getVisualParameter(paramNames[i]);

		float yBase = (paramCount * selectionBarWidth) / 2.0;

		float yMin = yBase + (-selectionBarWidth * i);
		float yMax = yBase + (-selectionBarWidth * (i + 1));

		if((x >= xMin) &&
			(x <= xMax)&&
			(y >= yMin) &&
			(y <= yMax))
		{
			name = paramNames[i];
			return true;
		}
	}

	name = "-1";
	return false;
}

bool WaveRadar::isInOpacityBar(float x, float y, int renderMode)
{
	float sliderDim;
	if(renderMode == WaveCanvas::RENDERMODE_SINGLERADAR)
	{
		sliderDim = opacitySliderDim = (radius - (radius * 1.333f)) * 0.5f;
	}
	else
	{
		sliderDim = -(1.0f / opacitySliderIncrements);
	}

	float xMin = offSideXPos + (sliderDim / 4.0f);
	float xMax = offSideXPos - (sliderDim / 4.0f);
	float yMin = sliderDim * opacitySliderIncrements / 2.0f;
	float yMax = -sliderDim * opacitySliderIncrements / 2.0f;

	return (x >= xMin) &&
		(x <= xMax)&&
		(y >= yMin) &&
		(y <= yMax);
}


float WaveRadar::getOpacitySliderPosition(float x, float y, int renderMode)
{
	float sliderDim;
	if(renderMode == WaveCanvas::RENDERMODE_SINGLERADAR)
	{
		sliderDim = opacitySliderDim = (radius - (radius * 1.333f)) * 0.5f;
	}
	else
	{
		sliderDim = 1.0f / opacitySliderIncrements;
	}

	float yMax = -sliderDim * opacitySliderIncrements / 2.0f;
	float position = (y - yMax) / (sliderDim * opacitySliderIncrements);

	if(position > 1.0f) position = 1.0f;
	else if(position < 0.0f) position = 0.0f;

	return position;
}

float WaveRadar::getOffsetTouchAngle(float a)
{
	if(playMode == PLAYMODE_RADAR)
	{
		return a + angleOffset;
	}
	else if(playMode == PLAYMODE_DISC)
	{
		return a + (playheadPosition - angleOffset);
	}
	else return -1;
}

float WaveRadar::howFarBetween(float ac, float a0, float a1)
{
	float l = WrappedComparison::compare(a0, a1);
	float d = WrappedComparison::compare(a0, ac);

	if(signof(l) == signof(d) && fabs(d) <= fabs(l))	// Between
	{
		return (l != 0.0f) ? (fabs(d) / fabs(l)) : 0.0f;
	}
	else if(signof(l) == signof(d))						// On the side of a1
	{
		return 2.0f;
	}
	else												// On the side of a0
	{
		return -1.0f;
	}

	return 0;
}

float WaveRadar::getNewDistance(int fId, string pName, float ac, float rc, float a0, float a1, float r0, float r1)
{
	float rd = 0.0f;
	float w = 0.0f;

	float fb = howFarBetween(ac, a0, a1);


	if(fb >= 0.0f && fb <= 1.0f)
	{
		// lerp between
		float mu = fb; // abs(a0 - a1) / ac;	
		rd = Interpolator::linearInterpolate(r0, r1, mu);

		w = 1.0f;
	}
	else 
	{
		float l = 0.0f;
		float fl = 0.0f;

		if (fb > 1.0f)	// on the side of a1
		{
			fl = getNearestNeighbour(fId, pName, a1, a0);

			rd = r1;
			l = fabs(WrappedComparison::compare(a1, ac));
		}
		else if (fb < 0.0f)	// on the side of a0
		{
			fl = getNearestNeighbour(fId, pName, a0, a1);

			rd = r0;
			l = fabs(WrappedComparison::compare(a0, ac));
		}

		// fraction along
		float f = (fl == 0.0f) ? 0.0f : (l / fl);
		if (f >= 0.0f && f <= 1.0f)
		{
			w = sigmoidDecay(1-f);
			//w = gaussianDecay(f);
			//w = cauchyDecay(f);
			//w = 1-f;
			//w = Interpolator::linearInterpolate(1.0f, 0.0f, f);
			//w = (1.0f - f);
		}
	}

	return ((1.0f - w) * rc) + (w * rd);
}

float WaveRadar::sigmoidDecay(float t)
{//printf("%f\n", t);
/*
	if(t>0.02) return 0.0;
	return 1.0;*/

	//return pow((float)(1-t),(float)45.0f);

   return 1.0 - 1.0 / (1.0 + pow(M_E, -abs(1-t) * 12.0 + 6.0));
}


float WaveRadar::gaussianDecay(float x)
{
	float a = 1.0f;// peak height
	float b = 0.0f; // centre position
	float c = 0.035f; // width of the bell

	return pow((float)(a * M_E), (float)-(((x - b) * (x - b)) / ((2 * c) * (2 * c)))); 
}

float WaveRadar::cauchyDecay(float x)
{
	// http://en.wikipedia.org/wiki/Cauchy_distribution
	// http://mathworld.wolfram.com/CauchyDistribution.html
	// DOESN'T WORK YET!

	float b = 0.05f; // half width
	float m = 0.0f; // median

	return (1.0f / M_PI) * (b / ((x - m) * (x - m)) + (b * b));
}

float WaveRadar::getNearestNeighbour(int fId, string pName, float ai, float aj)
{
	float distance = maxDistance;
	float aiToAj = WrappedComparison::compare(ai, aj);
	
	if(fingers.size() > 1)
	{
		for (map<int,UT::Finger>::iterator it = fingers.begin(); it != fingers.end(); it++)
		{
			int key = (*it).first;
			UT::Finger f = (*it).second;

			if(fId != f.fId && pName == f.parameter)
			{
				float apToAi = WrappedComparison::compare(f.lastControlA, ai);

				if(signof(aiToAj) == signof(apToAi))
				{
					if(distance > fabs(apToAi))
					{
						distance = fabs(apToAi);
					}
				}
			}
		}
	}
	return distance;
}

void WaveRadar::getPolarCoordinate(float x, float y, float &a, float &r)
{
	r = sqrt((y * y) + (x * x));
	a = atan2(y, x);

	if(a < 0)
	{
		a += (M_PI * 2);
	}
}

void WaveRadar::getCartesianCoordinate(float a, float r, float &x, float &y)
{
	x = r * cos(a);
	y = r * sin(a);
}

WaveVisual* WaveRadar::getWaveVisual()
{
	return visual;
}


void WaveRadar::setPosition(float x, float y)
{
	position.x = x;
	position.y = y;
}

float WaveRadar::toWaveSpace(float rad)
{
	float val = (outerBoundary - innerBoundary) * rad;
	return (val + innerBoundary) * radius;
}

float WaveRadar::toDegrees(float rad)
{
	if(rad == 0.0f) return 0.0f;
	else return rad * (180 / M_PI);
}

float WaveRadar::fromWaveSpace(float rad)
{
	float val = rad - (innerBoundary * radius);
	val = val * (1.0f / (outerBoundary - innerBoundary));
	val = val / radius;

	return val;
}

bool WaveRadar::findClosestWaveHit(float x, float y, string &parameterName)
{
	float minVal = -1.0f;
	string minName = "-1";

	int paramCount;
	string* params = visual->getParameters(paramCount);
	
	for(int i = 0; i < paramCount; i++)
	{
		// ignore if not selected
		if(visual->getVisualParameter(params[i])->getSelected())
		{
			int length;
			float* vals = visual->getVisualParameter(params[i])->getValues(length);		
			float incrementAngle = (M_PI * 2) / length;

			for(int j = 0; j < length; j++)
			{
				float ax, ay;
				getCartesianCoordinate(j * incrementAngle, toWaveSpace(vals[j]), ax, ay);

				float bx, by;
				getCartesianCoordinate(((j + 1) % length) * incrementAngle, toWaveSpace(vals[(j + 1) % length]), bx, by);

				float distanceSegment, distanceLine;
				distanceFromLine(x, y, ax, ay, bx, by, distanceSegment, distanceLine);

				if((minVal < 0.0f) || (minVal > distanceSegment))
				{
					minVal = distanceSegment;
					minName = params[i];
				}
			}
		}
	}

	parameterName = minName;
	return minVal <= hitThreshold;
}

void WaveRadar::distanceFromLine(float cx, float cy, float ax, float ay, float bx, float by, float &distanceSegment, float &distanceLine)
{
	float r_numerator = (cx-ax)*(bx-ax) + (cy-ay)*(by-ay);
	float r_denomenator = (bx-ax)*(bx-ax) + (by-ay)*(by-ay);
	float r = r_numerator / r_denomenator;

	float px = ax + r*(bx-ax);
	float py = ay + r*(by-ay);

	float s =  ((ay-cy)*(bx-ax)-(ax-cx)*(by-ay) ) / r_denomenator;

	distanceLine = fabs(s)*sqrt(r_denomenator);

	float xx = px;
	float yy = py;

	if ( (r >= 0) && (r <= 1) )
	{
		distanceSegment = distanceLine;
	}
	else
	{
		float dist1 = (cx-ax)*(cx-ax) + (cy-ay)*(cy-ay);
		float dist2 = (cx-bx)*(cx-bx) + (cy-by)*(cy-by);
	
		if (dist1 < dist2)
		{
			xx = ax;
			yy = ay;
			distanceSegment = sqrt(dist1);
		}
		else
		{
			xx = bx;
			yy = by;
			distanceSegment = sqrt(dist2);
		}
	}

	return;
}

/*
void WaveRadar::setAngleOffset(float angleOffset)
{
	WaveRadar::angleOffset = angleOffset;
}*/

void WaveRadar::togglePlayMode()
{
	if(playMode == PLAYMODE_RADAR)
	{
		playMode = PLAYMODE_DISC;
		angleOffset = playheadPosition - angleOffset;
	}
	else if(playMode == PLAYMODE_DISC)
	{
		playMode = PLAYMODE_RADAR;
		angleOffset = playheadPosition - angleOffset;
	}
}

void WaveRadar::render(float playheadPosition, int renderMode)
{
	WaveRadar::playheadPosition = playheadPosition;

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	float playheadRotation = 0.0f;
	if(playMode == PLAYMODE_RADAR)
	{
		playheadRotation = playheadPosition - angleOffset;
	}
	else if(playMode == PLAYMODE_DISC)
	{
		playheadRotation = angleOffset;
	}

	glRotatef(-toDegrees(WrappedComparison::wrapMod(playheadRotation, 2 * M_PI)), 0.0f, 0.0f, 1.0f);
	
	renderPlayhead();

	glPopMatrix();

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	
	// rotate background and waves
	float discRotation = 0.0f;
	if(playMode == PLAYMODE_RADAR)
	{
		discRotation = angleOffset;
	}
	else if(playMode == PLAYMODE_DISC)
	{
		discRotation = playheadPosition - angleOffset;
	}
	glRotatef(toDegrees(WrappedComparison::wrapMod(discRotation, 2 * M_PI)), 0.0f, 0.0f, 1.0f);
	
	renderBackground();
	renderWaves();

	glPopMatrix();

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	if(renderMode != WaveCanvas::RENDERMODE_MULTIPLERADARS)
	{
		renderSelectionBar();
	}

	renderOpacitySlider(renderMode);

	glPopMatrix();

	//dirtyFlagFlag(false);
	int length;
	string* paramNames = visual->getParameters(length);
	for(int i = 0; i < length; i++)
	{
		visual->getVisualParameter(paramNames[i])->setDirtyFlag(false);
	}
}


void WaveRadar::renderBackground()
{
	// generate display list
	if(backgroundDL == -1)
	{
		float boldAlpha = 0.65f;
		float boldThickness = 1.15f;
		float lightAlpha = 0.35f;
		float lightThickness = 0.65f;
		float innerSpokeBoundary = 0.175f;

		backgroundDL = glGenLists(1);

		/* DRAWING CODE */
		glNewList(backgroundDL, GL_COMPILE);

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glPushAttrib(GL_ALL_ATTRIB_BITS);

		// draw the line around the main circle
		glLineWidth(boldThickness);
		glColor4f(1.0f, 1.0f, 1.0f, boldAlpha);

		glBegin(GL_LINE_LOOP);
		for(int i = 0; i < 360; i++)
		{
			float offset = i * (M_PI / 180);
			glVertex3f(cos(offset)* (radius * outerBoundary), sin(offset) * (radius * outerBoundary), 0.0f);
		}
		glEnd();
		
		// draw the spokes
		glBegin(GL_LINES);
		for(int i = 0; i < (beats / 4); i++)
		{
			float offset = i * (M_PI / (beats / 8));
			
			if(i % 2 == 0)
			{
				glLineWidth(boldThickness);
				glColor4f(1.0f, 1.0f, 1.0f, boldAlpha);

				glVertex3f(cos(offset)* (radius * innerSpokeBoundary), sin(offset) * (radius * innerSpokeBoundary), 0.0f);
				glVertex3f(cos(offset)* (radius * outerBoundary), sin(offset) * (radius * outerBoundary), 0.0f);
			}
			else
			{
				glLineWidth(lightThickness);
				glColor4f(1.0f, 1.0f, 1.0f, lightAlpha);

				glVertex3f(cos(offset)* (radius * innerBoundary), sin(offset) * (radius * innerBoundary), 0.0f);
				glVertex3f(cos(offset)* (radius * outerBoundary), sin(offset) * (radius * outerBoundary), 0.0f);
			}
		}
		glEnd();

		// draw the concentric circles
		// 0.4, 0.51, 0.62, 0.73, 0.84 * rad
		glLineWidth(lightThickness);
		glColor4f(1.0f, 1.0f, 1.0f, lightAlpha);

		float incrementValue = (outerBoundary - innerBoundary) / increments; 
		float total = innerBoundary;

		for(int i = 0; i < increments; i++)
		{
			glBegin(GL_LINE_LOOP);
			for(int j = 0; j < 360; j++)
			{
				float offset = j * (M_PI / 180);
				glVertex3f(cos(offset)* (radius * total), sin(offset) * (radius * total), 0.0f);
			}
			glEnd();

			total += incrementValue;
		}

		// draw the inner circle
		// 0.29 * rad
		glLineWidth(boldThickness);
		glColor4f(1.0f, 1.0f, 1.0f, boldAlpha);

		glBegin(GL_LINE_LOOP);
		for(int j = 0; j < 360; j++)
		{
			float offset = j * (M_PI / 180);
			glVertex3f(cos(offset)* (radius * innerBoundary), sin(offset) * (radius * innerBoundary), 0.0f);
		}
		glEnd();

		// draw the outer region
		float beatLineSize = 0.985f;
		float quarterBeatLine = 0.97f;

		glBegin(GL_LINES);
		for(int i = 0; i < 256; i++)
		{
			glLineWidth(boldThickness);
			glColor4f(1.0f, 1.0f, 1.0f, boldAlpha);
			float offset = i * (M_PI / 128);

			if(i % 16 == 0)
			{
				glVertex3f(cos(offset)* (radius * outerBoundary), sin(offset) * (radius * outerBoundary), 0.0f);
				glVertex3f(cos(offset)* radius, sin(offset) * radius, 0.0f);
			}
			else if(i % 4 == 0)
			{
				glVertex3f(cos(offset)* (radius * outerBoundary), sin(offset) * (radius * outerBoundary), 0.0f);
				glVertex3f(cos(offset)* (radius * beatLineSize), sin(offset) * (radius * beatLineSize), 0.0f);
			}
			else
			{
				glLineWidth(lightThickness);
				glColor4f(1.0f, 1.0f, 1.0f, lightAlpha);

				glVertex3f(cos(offset)* (radius * outerBoundary), sin(offset) * (radius * outerBoundary), 0.0f);
				glVertex3f(cos(offset)* (radius * quarterBeatLine), sin(offset) * (radius * quarterBeatLine), 0.0f);
			}
		}
		glEnd();

		glPopAttrib();
		glPopMatrix();

		glEndList();
		/* END DRAWING CODE */
	}
	
	glCallList(backgroundDL);
}

void WaveRadar::renderWaves()
{
	if(visual != NULL)
	{
		int paramCount;
		string* paramNames = visual->getParameters(paramCount);

		for(int i = 0; i < paramCount; i++)
		{
			if(visual->getVisualParameter(paramNames[i])->isDirtyFlagSet())
			{
				UT::Colour3f c = visual->getVisualParameter(paramNames[i])->getColourValue();

				GLuint waveDL = visual->getVisualParameter(paramNames[i])->getDisplayList();

				if(waveDL == NULL)
				{
					waveDL = glGenLists(1);
					visual->getVisualParameter(paramNames[i])->setDisplayList(waveDL);
				}

				int length;
				float* param = visual->getVisualParameter(paramNames[i])->getValues(length);

				/* DRAWING CODE */
				glNewList(waveDL, GL_COMPILE);

				glMatrixMode(GL_MODELVIEW);
				glPushMatrix();
				glPushAttrib(GL_ALL_ATTRIB_BITS);

				// draw the line
				glLineWidth(7.5f);
				glColor4f(c.r, c.g, c.b, 0.45f);
				glBegin(GL_LINE_LOOP);
				for(int j = 0; j < length; j++)
				{
					for(int k = 0; k < upSampleFactor; k++)
					{
						float interpolatedValue = visual->getVisualParameter(paramNames[i])->getInterpolatedValue(j % length, k * (1.0f / upSampleFactor));
						float value = toWaveSpace(interpolatedValue);

						float offset = (((j % length) * upSampleFactor) + k)  * ((M_PI * 2) / (length * upSampleFactor));
						glVertex3f(cos(-offset) * value, sin(-offset) * value, 0.0f);
					}
				}
				glEnd();

				// overlay the beat values
				for(int j = 0; j < length; j++)
				{
					if(j % 4 == 0)
					{
						float interpolatedValue = visual->getVisualParameter(paramNames[i])->getInterpolatedValue(j % length, 0.0f);
						float value = toWaveSpace(interpolatedValue);

						float centrePoint = (j % length) * ((M_PI * 2) / length);

						float x, y;
						getCartesianCoordinate(-centrePoint, value, x, y);
/*
						glBegin(GL_POINTS);
							glVertex3f(cos(-centrePoint) * value, sin(-centrePoint) * value, 0.0f);
						glEnd();
*/
						glMatrixMode(GL_MODELVIEW);
						glPushMatrix();
						glTranslatef(x, y, 0.0f);

						float pointSize = 0.0055f;

						glLineWidth(1.0f);
						glColor4f(1.0f, 1.0f, 1.0f, 0.25f);
						glBegin(GL_LINE_LOOP);
						for(int k = 0; k < 32; k++)
						{
							float offset = k * ((2 * M_PI) / 32);
							glVertex3f(cos(-offset) * pointSize, sin(-offset) * pointSize, 0.0f);
						}
						glEnd();

						glColor4f(1.0f, 1.0f, 1.0f, 0.1f);
						for(int k = 0; k < 32; k++)
						{
							glBegin(GL_POLYGON);
							float offset0 = k * ((2 * M_PI) / 32);
							float offset1 = ((k % 32) + 1) * ((2 * M_PI) / 32);
							glVertex3f(0.0f, 0.0f, 0.0f);
							glVertex3f(cos(-offset0) * pointSize, sin(-offset0) * pointSize, 0.0f);
							glVertex3f(cos(-offset1) * pointSize, sin(-offset1) * pointSize, 0.0f);
							glEnd();						
						}
						glPopMatrix();
					}
				}

				glPopAttrib();
				glPopMatrix();

				glEndList();
				/* ENDDRAWING CODE */ 
			}

			glCallList(visual->getVisualParameter(paramNames[i])->getDisplayList());
		}
	}
}

void WaveRadar::renderPlayhead()
{
	if(playheadDL == -1)
	{
		playheadDL = glGenLists(1);

		/* DRAWING CODE */
		glNewList(playheadDL, GL_COMPILE);

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glPushAttrib(GL_ALL_ATTRIB_BITS);
	
		float opacity = 0.6f;
		// draw the leading line (to get an antialiased straight edge)
		glLineWidth(1.0f);
		glBegin(GL_LINE_STRIP);
			glColor4f(0.55f, 0.8f, 1.0f, opacity);
			glVertex3f(0.0f, (radius * outerBoundary), 0.0f);
			glColor4f(0.55f, 0.8f, 1.0f, 0.0f);
			glVertex3f(0.0f, 0.0f, 0.0f);
		glEnd();

		// draw the radar region		
		glBegin(GL_POLYGON);
			glColor4f(0.55f, 0.8f, 1.0f, opacity);
			glVertex3f(0.0f, (radius * outerBoundary), 0.0f);
			glColor4f(0.55f, 0.8f, 1.0f, 0.0f);

			float curOpacity = opacity;
			int length = 16;
			for(int i = 0; i < length; i++)
			{
				float offset = i * (((2 * M_PI) / (beats / length)) / length) + (M_PI / 2);
				curOpacity -= (opacity / (float)length);
				glColor4f(0.55f, 0.8f, 1.0f, curOpacity);
				glVertex3f(cos(offset) * (radius * outerBoundary), sin(offset) * (radius * outerBoundary), 0.0f);
			}
			glVertex3f(0.0f, 0.0f, 0.0f);
		glEnd();

		glPopAttrib();
		glPopMatrix();

		glEndList();
	}

	glCallList(playheadDL);
}


void WaveRadar::renderSelectionBar()
{
	if(selectionBarDL == -1 || dirtyFlag)
	{
		selectionBarDL = glGenLists(1);

		/* DRAWING CODE */
		glNewList(selectionBarDL, GL_COMPILE);

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glPushAttrib(GL_ALL_ATTRIB_BITS);

		// translate to the left
		glTranslatef(-offSideXPos, 0.0f, 0.0f);

		int paramCount;
		string* paramNames = visual->getParameters(paramCount);

		glTranslatef(0.0f, (-paramCount * selectionBarWidth) / 2.0f, 0.0f); 

		for(int i = 0; i < paramCount; i++)
		{
			VisualParameter* param = visual->getVisualParameter(paramNames[i]);

			float yPos = selectionBarWidth * i; 

			glMatrixMode(GL_MODELVIEW);
			glPushMatrix();
			glTranslatef(0.0f, yPos, 0.0f);			

			float boxDim = selectionBarWidth * 0.8f;

			// draw the line
			glMatrixMode(GL_MODELVIEW);
			glPushMatrix();
			glTranslatef(-(boxDim / 2.0f), 0.0f, 0.0f);

			UT::Colour3f c = param->getColourValue();
			glColor3f(c.r, c.g, c.b);
			glLineWidth(4.0f);

			float borderWidth = boxDim * 0.1f;
			float segments = 50.0f;
			float increment = (boxDim - (borderWidth * 2.0f)) / segments;

			glBegin(GL_LINE_STRIP);
				for(int i = 0; i < segments; i++)
				{
					float yVal = (boxDim / 5.0f) * cos(0.15f * i) + (boxDim / 2.0f);
					glVertex3f(borderWidth + (increment * i), yVal, 0.0f);
				}
			glEnd();

			glPopMatrix();

			// draw the box
			glColor4f(1.0f, 1.0f, 1.0f, 0.65);
			if(param->getSelected()) glLineWidth(1.75f);
			else glLineWidth(0.5f);

			glBegin(GL_LINE_LOOP);
				glVertex3f(-(boxDim / 2.0f), 0.0f, 0.0f);
				glVertex3f(boxDim / 2.0f, 0.0f, 0.0f);
				glVertex3f(boxDim / 2.0f, boxDim, 0.0f);
				glVertex3f(-(boxDim / 2.0f), boxDim, 0.0f);
			glEnd();

			
			if(param->getSelected()) glColor4f(0.55, 0.8f, 1.0f, 0.35f);
			else glColor4f(0.55, 0.8f, 1.0f, 0.15f);

			glBegin(GL_QUADS);
				glVertex3f(-(boxDim / 2.0f), 0.0f, 0.0f);
				glVertex3f(boxDim / 2.0f, 0.0f, 0.0f);
				glVertex3f(boxDim / 2.0f, boxDim, 0.0f);
				glVertex3f(-(boxDim / 2.0f), boxDim, 0.0f);
			glEnd();

	
			glPopMatrix();
		}

		glPopMatrix();
		glPopAttrib();

		glEndList();
	}

	glCallList(selectionBarDL);
}

void WaveRadar::renderOpacitySlider(int renderMode)
{
	float sliderDim;
	if(renderMode == WaveCanvas::RENDERMODE_SINGLERADAR)
	{
		sliderDim = opacitySliderDim = (radius - (radius * 1.333f)) * 0.5f;
	}
	else
	{
		sliderDim = -(1.0f / opacitySliderIncrements);
	}

	if(opacitySliderDL == -1 || dirtyFlag)
	{
		opacitySliderDL = glGenLists(1);

		/* DRAWING CODE */
		glNewList(opacitySliderDL, GL_COMPILE);

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glPushAttrib(GL_ALL_ATTRIB_BITS);

		glTranslatef(offSideXPos, -(sliderDim * opacitySliderIncrements) / 2.0f, 0.0f);

		// draw centre line
		glColor4f(1.0f, 1.0f, 1.0f, 0.65f);
		glLineWidth(1.15f);
		
		glBegin(GL_LINE_STRIP);
			glVertex3f(0.0f, 0.0f, 0.0f);
			glVertex3f(0.0f, sliderDim * opacitySliderIncrements, 0.0f);
		glEnd();

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();

		// draw the increments
		for(int i = 0; i <= opacitySliderIncrements; i++)
		{
			glColor4f(1.0f, 1.0f, 1.0f, 0.65f);
			glLineWidth(1.15f);

			glBegin(GL_LINE_STRIP);
				// thick line (every 5)
				glVertex3f(-sliderDim / 4.0f, 0.0f, 0.0f);
				glVertex3f(sliderDim / 4.0f, 0.0f, 0.0f);
			glEnd();

			// thin lines

			if(i != opacitySliderIncrements)
			{
				glColor4f(1.0f, 1.0f, 1.0f, 0.65f);
				glLineWidth(0.35f);
				float div = 5.0f;
				for(int j = 0; j < (div - 1); j++)
				{
					glTranslatef(0.0f, sliderDim / div, 0.0f);
				
					glBegin(GL_LINE_STRIP);
						glVertex3f(-sliderDim / 7.0f, 0.0f, 0.0f);
						glVertex3f(sliderDim / 7.0f, 0.0f, 0.0f);
					glEnd();
				}
				glTranslatef(0.0f, sliderDim / div, 0.0f);
			}
		}

		glPopMatrix();

		// draw the bar
		float opacity = visual->getOpacity();

		glColor4f(0.55, 0.8f, 1.0f, 0.35f);
		
		for(int i = 0; i < 2; i ++)
		{
			if(i == 0) glBegin(GL_POLYGON);
			else glBegin(GL_LINE_LOOP);
				glVertex3f(-sliderDim / 4.0f, (sliderDim * opacitySliderIncrements) - (sliderDim * opacitySliderIncrements) * opacity, 0.0f);
				glVertex3f(sliderDim / 4.0f, (sliderDim * opacitySliderIncrements) - (sliderDim * opacitySliderIncrements) * opacity, 0.0f);
				glVertex3f(sliderDim / 4.0f, sliderDim * opacitySliderIncrements, 0.0f);
				glVertex3f(-sliderDim / 4.0f, sliderDim * opacitySliderIncrements, 0.0f);
			glEnd();
		}

		glPopMatrix();
		glPopAttrib();

		glEndList();
		/* END DRAWING CODE */
	}

	glCallList(opacitySliderDL);
}