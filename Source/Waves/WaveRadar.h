#ifndef WaveRadar_H
#define WaveRadar_H

#include <string>
#include <map>
#include "WaveVisual.h"

using namespace std;

class WaveRadar
{
public:
	//WaveRadar(int beats);
	WaveRadar(int beats, int upSampleFactor, WaveVisual* visual, UT::Point2f position);
	~WaveRadar();

	static const int PLAYMODE_RADAR = 0;
	static const int PLAYMODE_DISC = 1;

	void render(float playheadPosition, int renderMode);
	void fingerAdded(int id, float x, float y);
	void fingerUpdated(int id, float x, float y);
	void fingerRemoved(int id, float x, float y);

	float getRadius();
	int getId();
	float getOpacitySliderPosition(float x, float y, int renderMode);

	bool isInOpacityBar(float x, float y, int renderMode);

	WaveVisual* getWaveVisual();
	UT::Point2f getPosition();

	void setPosition(float x, float y);

	void togglePlayMode();

private:
	bool dirtyFlag;

	float angleOffset;
	float hitThreshold;
	float innerBoundary;
	float maxDistance;
	float outerBoundary;
	float playheadPosition;
	float radius;
	float offSideXPos;
	float opacitySliderDim;
	float opacitySliderIncrements;
	float selectionBarWidth;

	GLuint backgroundDL;
	GLuint playheadDL;
	GLuint selectionBarDL;
	GLuint opacitySliderDL;

	int id;
	int beats;
	int increments;
	int playMode;
	int upSampleFactor;

	UT::Point2f position;

	map<int, UT::Finger> fingers;
	WaveVisual* visual;

	float cauchyDecay(float x);
	void distanceFromLine(float cx, float cy, float ax, float ay, float bx, float by, float &distanceSegment, float &distanceLine);
	bool findClosestWaveHit(float x, float y, string &parameterName);
	float fromWaveSpace(float rad);
	float gaussianDecay(float t);
	
	void getPolarCoordinate(float x, float y, float &a, float &r); 
	void getCartesianCoordinate(float a, float r, float &x, float &y);
	float getOffsetTouchAngle(float a);
	float getNewDistance(int fId, string pName, float ac, float rc, float a0, float a1, float r0, float r1);
	float getWeightedDistance(int fId, float ac, float a0, float a1, float rd);
	float getNearestNeighbour(int fId, string pName, float ai, float aj);


	float howFarBetween(float ac, float a0, float a1);


	bool isInCentre(float r);
	bool isInSelectionBox(float a, float r, string &name);


	void renderBackground();
	void renderWaves();
	void renderPlayhead();
	void renderSelectionBar();
	void renderOpacitySlider(int renderMode);

	void setVisual(WaveVisual* visual);
	void setAngleOffset(float angleOffset); 
	
	float sigmoidDecay(float t);
	float toWaveSpace(float rad);
	float toDegrees(float rad);

};
#endif