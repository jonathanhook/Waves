#include <stdio.h>
#include "JDHUtility/CrossPlatformTime.h"
#include "RunningAverage.h"
#include "TapBpm/TapBpm.h"

using namespace JDHUtility;

/****************
 * CONSTRUCTORS *
 ****************/
TapBpm::TapBpm(unsigned int numSamples, unsigned int resetPeriodMs)
{
	this->resetPeriodMs = resetPeriodMs;

	average = new RunningAverage(numSamples);
	init();
}

TapBpm::~TapBpm(void)
{
	delete average;
}

/***************************
 * PUBLIC MEMBER FUNCTIONS *
 ***************************/
float TapBpm::getBpm(void)
{
	int avgVal = average->getAverage();
	
	if(avgVal != 0) return (1000.0f / (float)average->getAverage()) * 60.0f;
	else return 0;
}

void TapBpm::reset(void)
{
	init();
}

void TapBpm::tap(void)
{
	unsigned int now = CrossPlatformTime::getTimeMillis();
	unsigned int difference = now - last;

	if(difference > resetPeriodMs) reset();
	else average->addValue(difference);

	last = now;
}

/****************************
 * PRIVATE MEMBER FUNCTIONS *
 ****************************/
void TapBpm::init(void)
{
	last = 0;
	average->reset();
}
