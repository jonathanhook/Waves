/**
 * Class:	main
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */
#include "Waves/Canvas.h"
#include "MultiTouchEvents/FingerEventArgs.h"
#include "JDHUtility/GLContext.h"
#include "JDHUtility/GlutInclude.h"
#include "Waves/InterfaceManager.h"
#include "MultiTouchEvents/TuioEventSource.h"
#include "Waves/WavesGlobalState.h"
#include "JDHUtility/WGLStuff.h"
#include "JDHUtility/WIN32Utils.h"
#include "MultiTouchEvents/WMTouchEventSource.h"

#ifdef _WIN32
#define _CRT_SECURE_NO_WARNINGS
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif

using namespace JDHUtility;
using namespace MultiTouchEvents;
using namespace Waves;

typedef enum
{
	MINIMISED,
	FULLSCREEN_1,
	FULLSCREEN_2
} ScreenMode;

const int DEFAULT_WIDTH = 800;
const int DEFAULT_HEIGHT = 600;
const char *GLUT_CLASS = "GLUT";
const int TUIO_PORT = 3333;
const char *INTERFACE_WINDOW_NAME = "Waves - UI";
const char *OUTPUT_WINDOW_NAME = "Waves - Output";

InterfaceManager *interfaceManager;
int interfaceWindow;
float interfaceHeight = 0.0f;
float interfaceWidth = 0.0f;
float outputHeight = 0.0f;
float outputWidth = 0.0f;
bool leftDown = false;
int outputWindow;
ScreenMode screenMode = MINIMISED;

void idle(void);
void initialiseWaves(void);
void keyboard(unsigned char key, int x, int y);
void motion(int x, int y);
void mouse(int button, int state, int x, int y);
void mouseToWorld(int x, int y, float &fx, float &fy);
void renderInterface(void);
void renderOutput(void);
void reshapeInterface(int width, int height);
void reshapeOutput(int width, int height);
void save(void);
void setupParallelProjection(int width, int height);
void setupPerspectiveProjection(int width, int height);
void toggleFullscreen(void);

void idle(void)
{
	assert(interfaceManager != NULL);

	glutSetWindow(interfaceWindow);
	glutPostRedisplay();

	// load all content on the first idle call (need to call after OpenGL initialises in the glut loop)
	static bool contentLoaded = false;
	if(!contentLoaded)
	{
		WavesGlobalState *wavesGlobalState = WavesGlobalState::getInstance(); 
		assert(wavesGlobalState != NULL);
		wavesGlobalState->loadContent();
		contentLoaded = true;
	}

	interfaceManager->update();

	glutSetWindow(outputWindow);
	glutPostRedisplay();
}

void initialiseWaves(void)
{
	interfaceManager = new InterfaceManager(false);
	
	HWND windowTarget = FindWindow(GLUT_CLASS, INTERFACE_WINDOW_NAME);

	TuioEventSource *tuioEventSource = new TuioEventSource(windowTarget, TUIO_PORT);
	WMTouchEventSource	*wmTouchEventSource	= new WMTouchEventSource(windowTarget);

	interfaceManager->registerEventSource(tuioEventSource);
	interfaceManager->registerEventSource(wmTouchEventSource);
}

void keyboard(unsigned char key, int x, int y)
{
	assert(interfaceManager != NULL);
	
	bool peekEnabled = false;
	switch(key)
	{
		case 'F':
		case 'f':
			toggleFullscreen();
			break;
		case 's':
		case 'S':
			save();
			break;
		case 'T':
		case 't':
			peekEnabled = interfaceManager->isPeekEnabled();
			interfaceManager->setPeekEnabled(!peekEnabled);
			break;
		case 27:
			exit(0);
	}
}

void motion(int x, int y)
{
	if(leftDown)
	{
		assert(interfaceManager != NULL);

		float fx, fy;
		mouseToWorld(x, y, fx, fy);
		interfaceManager->raiseEvent(1, fx, fy, FingerEventArgs::FINGER_UPDATED);
	}
}

void mouse(int button, int state, int x, int y)
{
	assert(interfaceManager != NULL);

	if(button == GLUT_LEFT)
	{
		float fx, fy;
		mouseToWorld(x, y, fx, fy);

		switch(state)
		{
			case GLUT_DOWN:
				leftDown = true;
				interfaceManager->raiseEvent(1, fx, fy, FingerEventArgs::FINGER_ADDED);
				break;
			case GLUT_UP:
				leftDown = false;
				interfaceManager->raiseEvent(1, fx, fy, FingerEventArgs::FINGER_REMOVED);
				break;
		}
	}
}

void mouseToWorld(int x, int y, float &fx, float &fy)
{
	fx = (float)x / (float)glutGet(GLUT_WINDOW_WIDTH);
	fy = (float)y / (float)glutGet(GLUT_WINDOW_WIDTH);
}

void renderInterface(void)
{
	assert(interfaceManager != NULL);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_BLEND); 
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	setupPerspectiveProjection(interfaceWidth, interfaceHeight);
	interfaceManager->renderVisuals(false);

	setupParallelProjection(interfaceWidth, interfaceHeight);
	interfaceManager->renderInterface();

	glutSwapBuffers();

#ifdef _DEBUG
	int error = glGetError();
	if(error != GL_NO_ERROR) 
	{
		//printf("OpenGL Error: %i\r\n", error);
	}
#endif
}

void renderOutput()
{
	assert(interfaceManager != NULL);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_BLEND); 
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	setupPerspectiveProjection(outputWidth, outputHeight);
	interfaceManager->renderVisuals(true);

	glutSwapBuffers();
}

void reshapeInterface(int width, int height)
{
	interfaceWidth = width;
	interfaceHeight = height;

	setupParallelProjection(interfaceWidth, interfaceHeight);
	glutPostRedisplay();
}

void reshapeOutput(int width, int height)
{	
	outputWidth = width;
	outputHeight = height;

	setupPerspectiveProjection(outputWidth, outputHeight);
	glutPostRedisplay();
}

void save(void)
{
	assert(interfaceManager != NULL);

	interfaceManager->save();
}

void setupParallelProjection(int width, int height)
{
	if(height == 0)
	{
		height = 1;
	}

	float ratio = (float)width / (float)height;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, width, height);	
	glOrtho(0.0f, 1.0f, 1.0f / ratio, 0.0, 0.0f, 100.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity(); 

	// drop back into clipping planes
	glTranslatef(0.0f, 0.0f, -1.0f);
}

void setupPerspectiveProjection(int width, int height)
{
	if(height == 0)
	{
		height = 1;
	}

	float ratio = (float)width / (float)height;
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, width, height);	
	gluPerspective(InterfaceManager::CAMERA_FOV, ratio, 1.0f, 100.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(0.0f, 0.0f, InterfaceManager::CAMERA_Z, 
			  0.0f, 0.0f, 0.0f,
			  0.0f, 1.0f, 0.0f);

	// scale to account for camera position
	glTranslatef(0.0f, 0.0f, -1.0f);
	glScalef(2.0f, 2.0f, 2.0f);
}

void toggleFullscreen(void)
{
	HWND hWnd;

	if(screenMode == MINIMISED || screenMode == FULLSCREEN_1)
	{
		glutSetWindow(screenMode == MINIMISED ? interfaceWindow : outputWindow);
		glutPositionWindow(0, 0);
		glutReshapeWindow(glutGet(GLUT_SCREEN_WIDTH), glutGet(GLUT_SCREEN_HEIGHT));

		hWnd = FindWindow(GLUT_CLASS, screenMode == MINIMISED ? INTERFACE_WINDOW_NAME : OUTPUT_WINDOW_NAME);
		WIN32Utils::setWindowBorder(hWnd, false);

		glutSetWindow(screenMode == MINIMISED ? outputWindow : interfaceWindow);
		Vector2f secondMonitorBounds = WIN32Utils::getSecondMonitorBounds();
		glutPositionWindow(glutGet(GLUT_SCREEN_WIDTH), 0);
		glutReshapeWindow(secondMonitorBounds.getX(), secondMonitorBounds.getY()); 

		hWnd = FindWindow(GLUT_CLASS, screenMode == MINIMISED ? OUTPUT_WINDOW_NAME : INTERFACE_WINDOW_NAME);
		WIN32Utils::setWindowBorder(hWnd, false);

		screenMode = (screenMode == MINIMISED ? FULLSCREEN_1 : FULLSCREEN_2);
	}
	else
	{
		glutSetWindow(interfaceWindow);
		glutPositionWindow(10, 25);
		glutReshapeWindow(DEFAULT_WIDTH, DEFAULT_HEIGHT); 

		hWnd = FindWindow(GLUT_CLASS, INTERFACE_WINDOW_NAME);
		WIN32Utils::setWindowBorder(hWnd, true);

		glutSetWindow(outputWindow);
		glutPositionWindow(10, 25);
		glutReshapeWindow(DEFAULT_WIDTH, DEFAULT_HEIGHT); 

		hWnd = FindWindow(GLUT_CLASS, OUTPUT_WINDOW_NAME);
		WIN32Utils::setWindowBorder(hWnd, true);

		screenMode = MINIMISED;
	}
}

int main(int argc, char *argv[])
{
	// process command line arguments
	WavesGlobalState::externalIpAddress = "127.0.0.1";
    for(unsigned int i = 0; i < argc; i++)
    {
        if(strcmp(argv[i], "-a") == 0)
        {
            i = (++i % argc);
            WavesGlobalState::externalIpAddress = argv[i];
        }
    }

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(DEFAULT_WIDTH, DEFAULT_HEIGHT);

	outputWindow = glutCreateWindow(OUTPUT_WINDOW_NAME);
	glutInitWindowPosition(0, 0);
	glutDisplayFunc(renderOutput);
	glutReshapeFunc(reshapeOutput);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutKeyboardFunc(keyboard);
	GLContext::initialiseCurrentContext();

	interfaceWindow = glutCreateWindow(INTERFACE_WINDOW_NAME);
	glutInitWindowPosition(0, 0);
	glutDisplayFunc(renderInterface);
	glutReshapeFunc(reshapeInterface);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutKeyboardFunc(keyboard);
	GLContext::initialiseCurrentContext();

	glutIdleFunc(idle); 

	initialiseWaves();

#	ifdef _WIN32
		bool vSyncAvailable = WGLStuff::setVSync(1);
#		ifdef _DEBUG
		if(vSyncAvailable) printf("vSync enabled\r\n");
#		endif
#	endif

	printf("\r\nControls:\r\nSet fullscreen:\t\t'f'\r\nShow touchpoints: \t't'\r\nExit application:\t'Esc'\r\n\r\n");

	glewInit();
	glutMainLoop();

	if(interfaceManager != NULL) delete interfaceManager;
	interfaceManager = NULL;
}