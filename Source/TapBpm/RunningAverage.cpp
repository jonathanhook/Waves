/**
 * RunningAverage -- Keeps track of a running average
 * Author: Jon Hook (j.d.hook@ncl.ac.uk)
 */
#include "RunningAverage.h"
#include <stdio.h>

/* CONSTRUCTORS */
RunningAverage::RunningAverage(unsigned int numSamples)
{
	if(numSamples <= 0) numSamples = DEFAULT_NUM_SAMPLES;
	this->numSamples = numSamples;
	samples = new int[this->numSamples];

	init();
}

RunningAverage::~RunningAverage(void)
{
	delete [] samples;
}

/* PUBLIC MEMBER VARIABLES */
void RunningAverage::addValue(int value)
{
	if(ptr >= numSamples)
	{
		total -= samples[(ptr % numSamples)];
	}

	total += value;
	samples[(ptr % numSamples)] = value;
	ptr++;
}

int RunningAverage::getAverage(void)
{
	int div = numSamples;
	if(ptr < numSamples) div = ptr;

	if(div > 0) return total / div;
	else return 0;
}
	
int RunningAverage::getSamplesRecorded(void)
{
	if(ptr >= numSamples) return numSamples;
	else return (ptr + 1);
}

void RunningAverage::reset(void)
{
	init();
}

/* PRIVATE MEMBER FUNCTIONS */
void RunningAverage::init(void)
{
	ptr = 0;
	total = 0;
}
