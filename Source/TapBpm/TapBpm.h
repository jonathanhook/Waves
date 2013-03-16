#ifndef TapBpm_H
#define TapBpm_H

class RunningAverage;

class TapBpm
{
public:
	static const unsigned int DEFAULT_NUM_SAMPLES = 50;
	static const unsigned int DEFAULT_RESET_PERIOD_MS = 2500;

	TapBpm(unsigned int numSamples = DEFAULT_NUM_SAMPLES, unsigned int resetPeriodMs = DEFAULT_RESET_PERIOD_MS);
	~TapBpm(void);

	float getBpm(void);
	void reset(void);
	void tap(void);

private:
	RunningAverage *average;
	unsigned int last;
	unsigned int resetPeriodMs;

	void init(void);
};
#endif

