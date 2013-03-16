#ifdef _WIN32
	#define _AFXDLL
	#include <afx.h>
	#include <windows.h>
#endif

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

#include <vector>
#include <list>
#define _USE_MATH_DEFINES	// allows M_PI to be used
#include <math.h>
#include "TuioListener.h"
#include "TuioClient.h"
#include "JDHUtility/WrappedComparison.h"
#include "WaveCanvas.h"

using namespace std;

typedef WaveVisual* (*FNPTR)();

WaveCanvas::WaveCanvas(int beats)
{
	WaveCanvas::beats = beats;

	// HACK
	UT::Point2f position;
	position.x = 0.25f;
	position.y = 0.25f;

	// HACK
	loadVisuals();

	// HACK some is anyway!:
	selectedRadar = 0;
	bpm = 128.0f;
	scale = 0.175f;
	clickThreshold = 0.005f;
	taskbarNumVisuals = 10; 
	taskbarOffset = 0.0f;
	taskbarYBase = -0.5f;

	//mode = RENDERMODE_SINGLERADAR;
	mode = RENDERMODE_MULTIPLERADARS;

	taskbarDL = -1;

	taskbarDirty = true;

	UT::setColour4f(taskbarColour, 0.55f, 0.8f, 1.0f, 0.5f);

	previewWindowPos.x = 0.1f;
	previewWindowPos.y = 0.1f;
	previewWindowSize = 0.2f;
}

WaveCanvas::~WaveCanvas()
{
	for(map<int,WaveRadar*>::iterator it = radars.begin(); it != radars.end(); it++)
	{
		delete (*it).second;
	}

	radars.clear();

	if(client != NULL)
	{
		client->disconnect();
		delete client;
	}
}

void WaveCanvas::reshapeInterface(int width, int height)
{
	// set both to update saved matrices
	setPerspectiveProjection(width, height, true);
	setParalellProjection(width, height);
}

void WaveCanvas::reshapeOutput(int width, int height)
{
	setPerspectiveProjection(width, height, false);
}

void WaveCanvas::setParalellProjection(int width, int height)
{
	if(height == 0)
	{
		height = 1;
	}

	float ratio = 1.0f * ((float)width / (float)height);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, width, height);	
	glOrtho(-0.5f * ratio, 0.5f * ratio, -0.5f, 0.5f, -1.0f, 1.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void WaveCanvas::setPerspectiveProjection(int width, int height, bool saveMatrices)
{
	if(height == 0)
	{
		height = 1;
	}

	float ratio = 1.0f * ((float)width / (float)height);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, width, height);	
	
	gluPerspective(45, ratio, 0.1f, 1000.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0f, 0.0f, 1.0f, 
		      0.0f, 0.0f, -1.0f,
			  0.0f, 1.0f, 0.0f);

	if(saveMatrices)
	{
		glGetDoublev(GL_MODELVIEW_MATRIX, modelView);
		glGetDoublev(GL_PROJECTION_MATRIX, project);
		glGetIntegerv(GL_VIEWPORT, view);
	}
}


void WaveCanvas::fingerAdded(int id, float x, float y)
{
	// finger rendering code (for debug and possibly main system)
	UT::Point2f rf;
	rf.x = x;
	rf.y = y;
	visFingers[id] = rf;

	if(mode == RENDERMODE_SINGLERADAR)
	{
		radars[selectedRadar]->fingerAdded(id, x, y);
	}
	else if(mode == RENDERMODE_MULTIPLERADARS)
	{
		float px, py;
		toPerspectiveCoords(x, y, px, py);

		UT::Finger finger;
		finger.fId = id;
		finger.downX = x;
		finger.downY = y;
		finger.lastX = x;
		finger.lastY = y;
		finger.anchoredRadar = -1;
		finger.hasAnchoredVisual = false;
		finger.anchoredPreviewWindow = false;

		for(map<int,WaveRadar*>::iterator it = radars.begin(); it != radars.end(); it++)
		{
			float tpx = (px - (*it).second->getPosition().x) / scale;
			float tpy = (py - (*it).second->getPosition().y) / scale;

			if((*it).second->isInOpacityBar(tpx, tpy, RENDERMODE_MULTIPLERADARS))
			{
				(*it).second->getWaveVisual()->setOpacity((*it).second->getOpacitySliderPosition(tpx, tpy, RENDERMODE_MULTIPLERADARS));
			}
		}

		if(fingers[id].fingerMode != UT::FINGER_OPACITY)
		{
			int radarId;
			WaveVisual* vParam;
			if(previewWindowContains(px, py))
			{
				finger.anchoredPreviewWindow = true;
				finger.previewAnchorX = px - previewWindowPos.x;
				finger.previewAnchorY = py - previewWindowPos.y;
			}
			else if(taskbarContains(x, y, vParam))
			{
				if(vParam != NULL)
				{
					finger.hasAnchoredVisual = true;
					finger.visual = vParam;
				}
			}
			else if(radarContains(px, py, radarId))
			{
				finger.anchoredRadar = radarId;
				finger.anchorX = px - radars[radarId]->getPosition().x;
				finger.anchorY = py - radars[radarId]->getPosition().y;
			}
		}

		fingers[id] = finger;
	}
}

void WaveCanvas::fingerUpdated(int id, float x, float y)
{
	// finger rendering code (for debug and possibly main system)
	if(visFingers.count(id) > 0)
	{
		visFingers[id].x = x;
		visFingers[id].y = y;
	}
	else
	{
		UT::Point2f rf;
		rf.x = x;
		rf.y = y;
		visFingers[id] = rf;
	}

	if(mode == RENDERMODE_SINGLERADAR)
	{
		radars[selectedRadar]->fingerUpdated(id, x, y);
	}
	else if(mode == RENDERMODE_MULTIPLERADARS)
	{
		if(fingers.count(id) > 0)
		{
			float px, py;
			toPerspectiveCoords(x, y, px, py);

			// if inside radar and no recorded anchor then assume the finger has just been dragged in
			int radarId;
			WaveVisual* vParam;
			
			for(map<int,WaveRadar*>::iterator it = radars.begin(); it != radars.end(); it++)
			{
				float tpx = (px - (*it).second->getPosition().x) / scale;
				float tpy = (py - (*it).second->getPosition().y) / scale;

				if((*it).second->isInOpacityBar(tpx, tpy, RENDERMODE_MULTIPLERADARS))
				{
					(*it).second->getWaveVisual()->setOpacity((*it).second->getOpacitySliderPosition(tpx, tpy, RENDERMODE_MULTIPLERADARS));
				}
			}
					
			if(previewWindowContains(px, py) && fingers[id].anchoredPreviewWindow)
			{
				previewWindowPos.x = px - fingers[id].previewAnchorX;
				previewWindowPos.y = py - fingers[id].previewAnchorY;
			}
			else if(taskbarContains(x, y, vParam))
			{
				// only allow movement if at least (numvisuals) on the taskbar
				if(taskbarCount > taskbarNumVisuals)
				{
					// UNTESTED, find out whether it works when we get more than 2 visuals!!
					float width = 1.0f * ((float)glutGet(GLUT_WINDOW_WIDTH) / (float)glutGet(GLUT_WINDOW_HEIGHT));

					float xDiff = x - fingers[id].lastX;
					float maxOffset = (taskbarCount - taskbarNumVisuals) * width;

					if(xDiff >= 0 && xDiff <= maxOffset) taskbarOffset += xDiff;
				}

			}
			else if(radarContains(px, py, radarId) &&  fingers[id].anchoredRadar == -1)
			{
				fingers[id].anchoredRadar = radarId;
				fingers[id].anchorX = px - radars[radarId]->getPosition().x;
				fingers[id].anchorY = py - radars[radarId]->getPosition().y;
			}


			if(fingers[id].hasAnchoredVisual)
			{
				UT::Point2f pos;
				pos.x = x;
				pos.y = -y;

				fingers[id].visual->setBeingDragged(true);
				fingers[id].visual->setPosition(pos);
			}
			else if(fingers[id].anchoredRadar != -1)
			{
				for(map<int,WaveRadar*>::iterator it = radars.begin(); it != radars.end(); it++)
				{
					if(fingers[id].anchoredRadar == (*it).second->getId() && hasMoved(x, y, fingers[id].downX, fingers[id].downY))
					{
						// do the drag
						(*it).second->setPosition(px - fingers[id].anchorX, py - fingers[id].anchorY);
					}
				}
			}

			fingers[id].lastX = x;
			fingers[id].lastY = y;
		}
		else fingerAdded(id, x, y);
	}
}

void WaveCanvas::fingerRemoved(int id, float x, float y)
{
	// finger rendering code (for debug and possibly main system)
	if(visFingers.count(id) > 0)
	{
		visFingers.erase(id);
	}

	if(mode == RENDERMODE_SINGLERADAR)
	{
		radars[selectedRadar]->fingerRemoved(id, x, y);
	}
	else if(mode == RENDERMODE_MULTIPLERADARS)
	{
		// if the finger has not moved (within a certain threshold) from when it was put down then assume a click event
		if(fingers.count(id) > 0)
		{
			float px, py;
			toPerspectiveCoords(x, y, px, py);

			int radarId;
			if(!hasMoved(x, y, fingers[id].downX, fingers[id].downY) && radarContains(px, py, radarId))
			{
				selectedRadar = radarId;
				mode = RENDERMODE_SINGLERADAR;
			}

			WaveVisual* vParam;
			if(fingers[id].hasAnchoredVisual)
			{
				if(taskbarContains(x, y, vParam))
				{
					fingers[id].visual->setBeingDragged(false);		
				}
				else
				{
					UT::Point2f pos;
					pos.x = px;
					pos.y = py;

					WaveRadar* wr = new WaveRadar(beats, 16, fingers[id].visual->createNewInstance(beats), pos);
					radars[wr->getId()] = wr;
					
					visuals.remove(fingers[id].visual);
				}
			}
			else if(taskbarContains(x, y, vParam) && 
					fingers[id].anchoredRadar != -1)
			{
				// 1) add that visual back to the list of potential visuals
				// MAY NOT WORK WITH OVER 10 VISUALS FOR NOW!
				float width = 1.0f * ((float)glutGet(GLUT_WINDOW_WIDTH) / (float)glutGet(GLUT_WINDOW_HEIGHT));
				float xDiff = x - -(width / 2);

				int index = xDiff / (width / taskbarNumVisuals);
				
				if(index >= visuals.size())
				{
					visuals.push_back(radars[fingers[id].anchoredRadar]->getWaveVisual()->createNewInstance(beats));
				}
				else
				{
					list<WaveVisual*>::iterator insertPosIt = visuals.begin();
					for(int i = 0; i < (index + 1); i++) insertPosIt++;
					visuals.insert(insertPosIt, radars[fingers[id].anchoredRadar]->getWaveVisual()->createNewInstance(beats));
				}

				// 2) remove radar
				map<int, WaveRadar*>::iterator it = radars.find(fingers[id].anchoredRadar);
				// Save?
				radars.erase(it);
			}

			fingers.erase(id);
		}
	}
}

bool WaveCanvas::hasMoved(float x, float y, float dx, float dy)
{
	dx = fabs(x - dx);
	dy = fabs(y - dy);

	return sqrt((dx * dx) + (dy * dy)) > clickThreshold;
}

void WaveCanvas::toPerspectiveCoords(float x, float y, float &px, float &py)
{
	float ratio = (float)glutGet(GLUT_WINDOW_WIDTH) / (float)glutGet(GLUT_WINDOW_HEIGHT);
	
	float wx = ((x / ratio) + 0.5f) * (float)glutGet(GLUT_WINDOW_WIDTH);
	float wy = (y + 0.5f) * (float)glutGet(GLUT_WINDOW_HEIGHT);

	float z = 0.0f; 
	//glReadPixels(wx, view[3] - wy, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &z);

	GLdouble dpx, dpy, dpz;
	gluUnProject(wx, view[3] - wy, z, modelView, project, view, &dpx, &dpy, &dpz);

	px = (float)dpx;
	py = (float)dpy;

	px = px * 10.0f;
	py = py * 10.0f;
}

bool WaveCanvas::radarContains(float x, float y, int &radarId)
{
	for(map<int,WaveRadar*>::iterator it = radars.begin(); it != radars.end(); it++)
	{
		float dx = fabs(x - (*it).second->getPosition().x);
		float dy = fabs(y - (*it).second->getPosition().y);

		float distance = sqrt((dx * dx) + (dy * dy));

		if(distance <= ((*it).second->getRadius() * scale))
		{
			radarId = (*it).second->getId();
			return true;
		}
	}

	radarId = -1;
	return false;
}

bool WaveCanvas::previewWindowContains(float x, float y)
{
	float px, py;
	toPerspectiveCoords(x, y, px, py);

	float minX = -((previewWindowSize * 1.33f) / 2.0f) + previewWindowPos.x;
	float maxX = ((previewWindowSize * 1.33f) / 2.0f) + previewWindowPos.y;

	float minY = -(previewWindowSize / 2.0f) + previewWindowPos.y;
	float maxY = (previewWindowSize / 2.0f) + previewWindowPos.y;

	return x >= minX && x <= maxX && y >= minY && y <= maxY;
}

bool WaveCanvas::taskbarContains(float x, float y, WaveVisual* &vParam)
{
	bool inBar = false;
	vParam = NULL;

	float ratio = (float)glutGet(GLUT_WINDOW_WIDTH) / (float)glutGet(GLUT_WINDOW_HEIGHT);
	float width = 1.0f * ratio;
	float height = 1.0f / (float)taskbarNumVisuals;

	float minX = -(width / 2);
	float maxX = width / 2;
	float minY = taskbarYBase;
	float maxY = taskbarYBase + height;

	if(x >= minX && x <= maxX && -y >= minY && -y <= maxY) 
	{
		inBar = true;

		int count = 0;
		for(list<WaveVisual*>::iterator it = visuals.begin(); it != visuals.end(); it++)
		{	
			float iconMinX = (-(width / 2) + ((height * 1.33f) * count)) + taskbarOffset;
			float iconMaxX = iconMinX + (height * 1.33f);

			if(x >= iconMinX && x <= iconMaxX) vParam = (*it);

			count++;
		}
	}

	return inBar;
}

float WaveCanvas::getPlayheadAngle()
{
	int currentTime = glutGet(GLUT_ELAPSED_TIME);
	int diff = currentTime - startTime;
	long loopTime = (60000.0f / bpm) * beats;
	long progress = diff % loopTime;

	return ((2 * M_PI) / (loopTime)) * progress;
}

void WaveCanvas::renderFingers()
{
	glPointSize(5.0f);
	glBegin(GL_POINTS);
	for(map<int,UT::Point2f>::iterator it = visFingers.begin(); it != visFingers.end(); it++)
	{
		glVertex3f(visFingers[(*it).first].x, -visFingers[(*it).first].y, 0.0f);
	}
	glEnd();
}

void WaveCanvas::renderInterface()
{
	glEnable(GL_TEXTURE_2D);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	glEnable(GL_BLEND); 
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_LINE_SMOOTH);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	// 1) Render Radars and output screen
	if(mode == RENDERMODE_SINGLERADAR)
	{
		// render the visual
		setPerspectiveProjection(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT), false);
		radars[selectedRadar]->getWaveVisual()->render(WrappedComparison::wrapMod(getPlayheadAngle() - (M_PI / 2), (2 * M_PI)));

		// render the radar
		setParalellProjection(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
		renderFingers();
		radars[selectedRadar]->render(getPlayheadAngle(), RENDERMODE_SINGLERADAR);
	}
	else if(mode == RENDERMODE_MULTIPLERADARS)
	{
		// render the central screen

		for(map<int,WaveRadar*>::iterator it = radars.begin(); it != radars.end(); it++)
		{
			glMatrixMode(GL_MODELVIEW);
			glPushMatrix();
			
			UT::Point2f position = (*it).second->getPosition();

			setPerspectiveProjection(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT), false);						
			glTranslatef(position.x, position.y, 0.0f);	
			glScalef(scale, scale, scale);

			(*it).second->getWaveVisual()->render(WrappedComparison::wrapMod(getPlayheadAngle() - (M_PI / 2), (2 * M_PI)));

			// render the radar
			(*it).second->render(getPlayheadAngle(), RENDERMODE_MULTIPLERADARS);

			glPopMatrix();
		}
	}

	// 2) render the taskbar and preview screen
	if(mode == RENDERMODE_MULTIPLERADARS)
	{
		setPerspectiveProjection(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT), false);
		renderPreviewWindow();

		setParalellProjection(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
		renderTaskbar();
		renderTaskbarIcons();
	}

	glPopMatrix();
}

void WaveCanvas::renderOutput()
{
	glEnable(GL_TEXTURE_2D);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glEnable(GL_BLEND); 
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_LINE_SMOOTH);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	setPerspectiveProjection(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT), false);

	for(map<int,WaveRadar*>::iterator it = radars.begin(); it != radars.end(); it++)
	{
		(*it).second->getWaveVisual()->render(WrappedComparison::wrapMod(getPlayheadAngle() - (M_PI / 2), (2 * M_PI)));
	}

	glPopMatrix();
}

void WaveCanvas::renderPreviewWindow()
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glPushAttrib(GL_ALL_ATTRIB_BITS);

	glTranslatef(previewWindowPos.x, previewWindowPos.y, 0.0f);
	glScalef(previewWindowSize, previewWindowSize, previewWindowSize);

	float ratio = (float)glutGet(GLUT_WINDOW_WIDTH) / (float)glutGet(GLUT_WINDOW_HEIGHT);
	float width = 1.0f * ratio;
	float height = 1.0f;
	
	for(int i = 0; i < 2; i++)
	{
		if(i == 1)
		{
			glColor4f(taskbarColour.r, taskbarColour.g, taskbarColour.b, taskbarColour.a);
			glLineWidth(1.0f);
			glBegin(GL_LINE_LOOP);
		}
		else
		{
			glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
			glBegin(GL_POLYGON);
		}
			glVertex3f(-(width / 2.0f), -(height / 2.0f), 0.0f);
			glVertex3f(width / 2.0f, -(height / 2.0f), 0.0f);
			glVertex3f(width / 2.0f, height / 2.0f, 0.0f);
			glVertex3f(-(width / 2.0f), height / 2.0f, 0.0f);
		glEnd();
	}

	for(map<int,WaveRadar*>::iterator it = radars.begin(); it != radars.end(); it++)
	{
		(*it).second->getWaveVisual()->render(WrappedComparison::wrapMod(getPlayheadAngle() - (M_PI / 2), (2 * M_PI)));
	}

	glPopMatrix();
	glPopAttrib();
}

void WaveCanvas::renderTaskbar()
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glPushAttrib(GL_ALL_ATTRIB_BITS);

	// 1) Draw the bar itself
	float ratio = (float)glutGet(GLUT_WINDOW_WIDTH) / (float)glutGet(GLUT_WINDOW_HEIGHT);
	float width = 1.0f * ratio;
	float height = 1.0f / (float)taskbarNumVisuals;

	glColor4f(taskbarColour.r, taskbarColour.g, taskbarColour.b, taskbarColour.a);	
	glBegin(GL_POLYGON);
		glVertex3f(-(width / 2.0f), taskbarYBase, 0.0f);
		glVertex3f(width / 2.0f, taskbarYBase, 0.0f);
		glVertex3f(width / 2.0f, taskbarYBase + height, 0.0f);
		glVertex3f(-(width / 2.0f), taskbarYBase + height, 0.0f);
	glEnd();

	glLineWidth(1.0f);
	glColor4f(taskbarColour.r, taskbarColour.g, taskbarColour.b, 1.0f);
	glBegin(GL_LINE_STRIP);
		glVertex3f(width / 2.0f, taskbarYBase + height, 0.0f);
		glVertex3f(-(width / 2.0f), taskbarYBase + height, 0.0f);
	glEnd();

	glPopMatrix();
	glPopAttrib();
	
}

void WaveCanvas::renderTaskbarIcons()
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glPushAttrib(GL_ALL_ATTRIB_BITS);

	float ratio = (float)glutGet(GLUT_WINDOW_WIDTH) / (float)glutGet(GLUT_WINDOW_HEIGHT);
	float width = 1.0f * ratio;
	float height = 1.0f / (float)taskbarNumVisuals;
	
	glTranslatef((-(width / 2) + ((height * 1.33f) / 2)) + taskbarOffset, taskbarYBase + (height / 2), -0.0005f);

	for(list<WaveVisual*>::iterator it = visuals.begin(); it != visuals.end(); it++)
	{
		if(!(*it)->isBeingDragged())
		{
			glMatrixMode(GL_MODELVIEW);
			glPushMatrix();
			float scaleFactor = width / (taskbarNumVisuals + 2);
			glScalef(scaleFactor / 2, scaleFactor / 2, scaleFactor / 2);

			// set all params to 0.5f
			(*it)->setOpacity(1.0f);
			(*it)->setAllParams(1.0f, false);
			(*it)->render(0);

			glPopMatrix();
		}

		glTranslatef(height * 1.33f, 0.0f, 0.0f);
	}

	glPopMatrix();

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	
	for(list<WaveVisual*>::iterator it = visuals.begin(); it != visuals.end(); it++)
	{
		if((*it)->isBeingDragged())
		{
			glMatrixMode(GL_MODELVIEW);
			glPushMatrix();

			UT::Point2f pos = (*it)->getPosition();
			glTranslatef(pos.x, pos.y, 0.0f);

			float scaleFactor = width / (taskbarNumVisuals + 2);
			glScalef(scaleFactor / 2, scaleFactor / 2, scaleFactor / 2);

			// set all params to 0.5f
			(*it)->setOpacity(1.0f);
			(*it)->setAllParams(1.0f, false);
			(*it)->render(0);

			glPopMatrix();
		}
	}

	glPopMatrix();
	glPopAttrib();
}

void WaveCanvas::setBpm(float bpm)
{
	WaveCanvas::bpm = bpm;	
}

void WaveCanvas::setPlayMode(bool playMode)
{
	// hack
	radars[selectedRadar]->togglePlayMode();
}

void WaveCanvas::start(int port)
{
	client = new TuioClient(port);
	client->addTuioListener(this);
	client->connect(false);

	//startTime = TimeMillis::getCurrentTimeMillis();
	startTime = glutGet(GLUT_ELAPSED_TIME);
}

void WaveCanvas::loadVisuals()
{
#ifdef _DEBUG
	char* vpath = "..\\Debug";
#else
	char vpath = "..\\Release";
#endif
	
#ifdef _WIN32
	char cmd[1024];
	sprintf(cmd, "%s\\*.dll", vpath);

	CFileFind finder;
	bool working = finder.FindFile(cmd);

	while(working)
	{
		working = finder.FindNextFile();
		HINSTANCE hDLL = LoadLibrary((LPCSTR)finder.GetFileName());

		if(hDLL != NULL)
		{
			FNPTR ptr;
			ptr = (FNPTR)GetProcAddress(hDLL, "createInstance");

			if(ptr != NULL)
			{
				visuals.push_back(ptr());
			}
		}
	}

	taskbarCount = visuals.size();
#else
#error I'm !a PC
#endif
}



/** TUIO STUFF */


void WaveCanvas::addTuioObject(TuioObject *tobj) 
{

}

void WaveCanvas::updateTuioObject(TuioObject *tobj) 
{

}

void WaveCanvas::removeTuioObject(TuioObject *tobj) 
{

}

void WaveCanvas::toWorldCoord(float x, float y,  float &fx, float &fy)
{
	float ratio = glutGet(GLUT_WINDOW_WIDTH) / glutGet(GLUT_WINDOW_HEIGHT);
	fx = x - (ratio / 2);
	fy = y - 0.5f;
}

void WaveCanvas::addTuioCursor(TuioCursor *tcur)
{
	float fx, fy;
	toWorldCoord(tcur->getX(), tcur->getY(), fx, fy);
	fingerAdded(tcur->getCursorID(), fx, fy);
}

void WaveCanvas::updateTuioCursor(TuioCursor *tcur) 
{
	float fx, fy;
	toWorldCoord(tcur->getX(), tcur->getY(), fx, fy);
	fingerUpdated(tcur->getCursorID(), fx, fy);
}

void WaveCanvas::removeTuioCursor(TuioCursor *tcur)
{
	float fx, fy;
	toWorldCoord(tcur->getX(), tcur->getY(), fx, fy);
	fingerRemoved(tcur->getCursorID(), fx, fy);
}

void WaveCanvas::refresh(TuioTime frameTime) 
{

}

/* END TUIO */