/**
 * Class:	LiveFFT
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */
#ifndef LiveFFT_H
#define LiveFFT_H

#include <string>
#include <vector>
#include "JDHUtility/RollingAverage.h"
#include "RtAudio.h"

using namespace JDHUtility;

class ofxFftBasic;

namespace FFT
{
	class LiveFFT
	{
	public:
		LiveFFT(unsigned int points = DEFAULT_POINTS, unsigned int averageWindow = DEFAULT_AVERAGE_WINDOW, float gain = DEFAULT_GAIN);
		~LiveFFT(void);

		const RollingAverage<float> *getAveragedOutput(void) const;
		const RollingAverage<float> *getAveragedMusicalRangeOutput(unsigned int &binCount) const;
		unsigned int getBinCount(void) const;
		unsigned int getBinIndex(unsigned int hz) const;
		float getDecayFactor(void) const;
		float getGain(void) const;
		const float *getMusicalRangeOutput(unsigned int &binCount) const;
		const float *getOutput(void) const;
		bool isInitialised(void) const;
		void setDecayFactor(float decayFactor);
		void setGain(float gain);
		void start(void);
		void stop(void);

	private:
		static const unsigned int DEFAULT_AVERAGE_WINDOW = 5;
		static const float DEFAULT_GAIN;
		static const unsigned int DEFAULT_POINTS = 768;
		static const unsigned int MUSIC_MAX_FREQ = 2000;
		static const unsigned int MUSIC_MIN_FREQ = 0;
		static const unsigned int SAMPLE_RATE = 44100;

		static int audioCallback(void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames, double streamTime, RtAudioStreamStatus status, void *userData);

		RollingAverage<float> *average;
		float decayFactor;
		float *eqFunction;
		ofxFftBasic *fft;
		float gain;
		bool initialised;
		unsigned int musicalRangePtr;
		unsigned int musicalRangeBinCount;
		float *output;
		unsigned int points;
		RtAudio *rtAudio;
		float *samples;
		bool streamOpen;

		void computeFFT(void);
		void computePersistantOutput(void);
		void initAudio(void);
	};
}
#endif

