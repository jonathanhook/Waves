/**
 * Class:	LiveFFT
 * Author:	Jonathan David Hook
 * Email:	j.d.hook@ncl.ac.uk
 * Web:		http://homepages.cs.ncl.ac.uk/j.d.hook
 */
#include "FFT/LiveFFT.h"
#include "RtError.h"
#include "FFT/ofxFft/ofxFft.h"
#include "FFT/ofxFft/ofxFftBasic.h"

namespace FFT
{
	/* PRIVATE CONSTANTS */
	const float LiveFFT::DEFAULT_GAIN = 80.0f;

	/* PRIVATE STATIC FUNCTIONS */
	int LiveFFT::audioCallback(void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames, double streamTime, RtAudioStreamStatus status, void *userData)
	{
		LiveFFT *liveFFT = (LiveFFT *)userData;
		float *input = (float *)inputBuffer;
		memcpy(liveFFT->samples, input, sizeof(float) * nBufferFrames);
		liveFFT->computeFFT();
		return 0;
	}

	/* CONSTRUCTORS */
	LiveFFT::LiveFFT(unsigned int points, unsigned int averageWindow, float gain)
	{
		this->points = points;
		this->gain = gain;

		fft = (ofxFftBasic *)ofxFft::create(points, OF_FFT_WINDOW_HAMMING);	

		average = new RollingAverage<float>[fft->getBinSize()];
		eqFunction = new float[fft->getBinSize()];
		initialised = false;
		musicalRangePtr = getBinIndex(MUSIC_MIN_FREQ);
		musicalRangeBinCount = getBinIndex(MUSIC_MAX_FREQ) - musicalRangePtr;
		output = new float[fft->getBinSize()];
		samples = new float[points];
		streamOpen = false;

		for(int i = 0; i < fft->getBinSize(); i++)
		{
			average[i] = *new RollingAverage<float>(averageWindow);
		}

		for(int i = 0; i < fft->getBinSize(); i++)
		{
			float x = (float)(i + 1) / (float)fft->getBinSize();
			eqFunction[i] = powf(x, 0.5f);
		}

		initAudio();
	}

	LiveFFT::~LiveFFT(void)
	{
		delete [] average;
		delete [] eqFunction;
		delete fft;
		delete [] output;
		delete rtAudio;
		delete [] samples;
	}

	/* PUBLIC MEMBER FUNCTIONS */
	const RollingAverage<float> *LiveFFT::getAveragedOutput(void) const
	{
		return average;
	}

	const RollingAverage<float> *LiveFFT::getAveragedMusicalRangeOutput(unsigned int &binCount) const
	{
		binCount = musicalRangeBinCount;
		return &average[musicalRangePtr];
	}

	unsigned int LiveFFT::getBinIndex(unsigned int hz) const
	{
		if(hz <= SAMPLE_RATE / 2)
		{
			return (unsigned int)((float)hz / (float)SAMPLE_RATE * (float)points);
		}
		else return 0;
	}

	unsigned int LiveFFT::getBinCount(void) const
	{
		return fft->getBinSize();
	}
	
	float LiveFFT::getDecayFactor(void) const
	{
		return decayFactor;
	}

	float LiveFFT::getGain(void) const
	{
		return gain;
	}

	const float *LiveFFT::getMusicalRangeOutput(unsigned int &binCount) const
	{
		binCount = musicalRangeBinCount;
		return &output[musicalRangePtr];
	}

	const float *LiveFFT::getOutput(void) const
	{
		return output;
	}

	bool LiveFFT::isInitialised(void) const
	{
		return initialised;
	}

	void LiveFFT::setDecayFactor(float decayFactor)
	{
		this->decayFactor = decayFactor;
	}

	void LiveFFT::setGain(float gain)
	{
		this->gain = gain;
	}

	void LiveFFT::start(void)
	{
		if(initialised && !streamOpen)
		{
			unsigned int deviceCount = rtAudio->getDeviceCount();
			for(unsigned int i = 0; i < deviceCount; i++)
			{
				RtAudio::DeviceInfo info = rtAudio->getDeviceInfo(i);
			
				if(info.isDefaultInput)
				{
					RtAudio::StreamParameters parameters;
					parameters.deviceId = i;
					parameters.nChannels = 1;
					parameters.firstChannel = 0;
					unsigned int bufferFrames = points;

					try 
					{
						rtAudio->openStream(NULL, 
									&parameters, 
									RTAUDIO_FLOAT32,
									SAMPLE_RATE,
									&bufferFrames,
									&LiveFFT::audioCallback,
									this);
						
						rtAudio->startStream();
					}
					catch (RtError& e) 
					{ 
#ifdef DEBUG
						printf("FFT error: could not open default audio input device\r\n"); 
#endif
					}
				}
			}
		}
	}

	void LiveFFT::stop(void)
	{
		if(streamOpen)
		{
			rtAudio->stopStream();
			rtAudio->closeStream();
		}
	}

	/* PRIVATE MEMBER FUNCTIONS */
	void LiveFFT::computeFFT(void)
	{	
		fft->setSignal(samples);
		fft->clampSignal();
		memcpy(output, fft->getAmplitude(), sizeof(float) * fft->getBinSize());

		for(int i = 0; i < fft->getBinSize(); i++)
		{
			output[i] = (output[i] * eqFunction[i]) * gain;
			(&average[i])->addValue(output[i]);
		}
	}

	void LiveFFT::initAudio(void)
	{
		try 
		{
			rtAudio = new RtAudio();
			initialised = true;
		}
		catch (RtError &error) 
		{
#ifdef DEBUG
			printf("FFT error: could not initialise RtAudio\r\n"); 
#endif
		}
	}
}
