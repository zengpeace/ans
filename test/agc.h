#ifndef _AGC_H_
#define _AGC_H_


#include <iostream>
#include <time.h>
#include "stdio.h"
#include "gain_control.h"

#define LOGD printf


class Agc
{
public:
	Agc(const int minLevel, const int maxLevel, const int mode, const int sampleRate, const WebRtcAgc_config_t &config, const int echo);
	~Agc();

	int deal(short *inData, short *outData);

private:
	Agc();

	void* _handle;
	int _echo;
	short _frameSize;
	int _micLevelIn;
	int _micLevelOut;
	unsigned char _saturationWarning;
};


class AgcImpl
{
public:
	AgcImpl(const int mode, const int sampleRate, const int echo, const short compressionGaindB = 9);
	~AgcImpl();

	int deal(short *inData, short *outData);

private:
	AgcImpl();

	Agc* _agc;
};


#endif

