/**
 * RunningAverage -- Keeps track of a running average
 * Author: Jon Hook (j.d.hook@ncl.ac.uk)
 */
#ifndef RunningAverage_H
#define RunningAverage_H

class RunningAverage
{
public:
	static const int DEFAULT_NUM_SAMPLES = 10;

	RunningAverage(unsigned int numSamples = DEFAULT_NUM_SAMPLES);
	~RunningAverage(void);

	void addValue(int value);
	int getAverage(void);
	int getSamplesRecorded(void);
	void reset(void);
private:
	unsigned int numSamples;
	unsigned int ptr;
	int *samples;
	int total;

	void init(void);
};
#endif

