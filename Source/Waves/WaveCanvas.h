#ifndef WaveCanvas_H
#define WaveCanvas_H

#include <time.h>
#include <string>
#include <vector>
#include <list>
#include "TuioListener.h"
#include "TuioClient.h"
#include "UT.h"
#include "WaveRadar.h"

using namespace std;
using namespace TUIO;



class WaveCanvas : public TuioListener
{
public:
	static const int RENDERMODE_MULTIPLERADARS = 0;
	static const int RENDERMODE_SINGLERADAR = 1;


	WaveCanvas(int beats);
	~WaveCanvas();

	void fingerAdded(int id, float x, float y);
	void fingerUpdated(int id, float x, float y);
	void fingerRemoved(int id, float x, float y);

	void renderFingers();
	void renderInterface();
	void renderOutput();
	void reshapeInterface(int width, int height);
	void reshapeOutput(int width, int height);

	void setBpm(float bpm);
	void setPlayMode(bool playMode);
	void start(int port);

	void addTuioObject(TuioObject *tobj);
	void updateTuioObject(TuioObject *tobj);
	void removeTuioObject(TuioObject *tobj);
	void addTuioCursor(TuioCursor *tcur);
	void updateTuioCursor(TuioCursor *tcur);
	void removeTuioCursor(TuioCursor *tcur);
	void refresh(TuioTime frameTime);

private:
	bool taskbarDirty;

	float bpm;	
	float scale;
	float clickThreshold;
	float taskbarOffset;
	float taskbarYBase;
	float previewWindowSize;

	GLuint taskbarDL;

	int beats;
	int mode;
	int selectedRadar;
	int startTime;
	int taskbarNumVisuals;
	int taskbarCount;

	map<int, UT::Finger>fingers;
	map<int, WaveRadar*> radars;
	map<int, UT::Point2f> visFingers;

	list<WaveVisual*> visuals;

	TuioClient* client;

	GLdouble modelView[16];
	GLdouble project[16];
	GLint view[4];

	UT::Colour4f taskbarColour;
	UT::Point2f previewWindowPos;

	bool radarContains(float x, float y, int &radarId);
	bool hasMoved(float x, float y, float dx, float dy);
	bool taskbarContains(float x, float y, WaveVisual* &vParam);
	bool previewWindowContains(float x, float y);

	float getPlayheadAngle();

	void setParalellProjection(int width, int height);
	void setPerspectiveProjection(int width, int height, bool saveMatrices);

	void renderTaskbar();
	void renderTaskbarIcons();
	void renderPreviewWindow();

	void toPerspectiveCoords(float x, float y, float &px, float &py);
	void toWorldCoord(float x, float y, float &fx, float &fy);

	void loadVisuals();
};
#endif
