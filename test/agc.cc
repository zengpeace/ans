#include "agc.h"


Agc::Agc(const int minLevel, const int maxLevel, const int mode, const int sampleRate, const WebRtcAgc_config_t &config, const int echo)
{
	int ret;
	ret = WebRtcAgc_Create(&_handle);	
	if(ret < 0)
	{
		LOGD("WebRtcAgc_Create fail ! ret = %d\n", ret);
		return;
	}

	ret = WebRtcAgc_Init(_handle, minLevel, maxLevel, mode, sampleRate);
	if(ret < 0)
	{
		LOGD("WebRtcAgc_Init fail ! ret = %d\n", ret);
		this->~Agc();
		return;
	}	

	ret = WebRtcAgc_set_config(_handle, config);
	if(ret < 0)
	{
		LOGD("WebRtcAgc_set_config fail ! ret = %d\n", ret);
		this->~Agc();
		return;
	}

	_echo = echo;
	_frameSize = sampleRate / 100;
	_micLevelIn = 0;
	_micLevelOut = 0;
	_saturationWarning = 0;
}

Agc::~Agc()
{
	if(_handle)
	{
		WebRtcAgc_Free(_handle);
		_handle = NULL;
	}
}


int Agc::deal(short *inData, short *outData)
{
	int ret;
	ret = WebRtcAgc_Process(_handle, inData, NULL, _frameSize, outData, NULL, _micLevelIn, &_micLevelOut, _echo, &_saturationWarning);
	if(ret < 0)
	{
		LOGD("WebRtcAgc_Process fail ! ret = %d\n", ret);
		return -1;
	}	

	_micLevelIn = _micLevelOut;
	return 0;
}


AgcImpl::AgcImpl(const int mode, const int sampleRate, const int echo, const short compressionGaindB)
{
	WebRtcAgc_config_t config;
	config.targetLevelDbfs = 3;
	config.compressionGaindB = compressionGaindB;
	config.limiterEnable = 1;

	_agc = new Agc(0, 255, mode, sampleRate, config, echo);
	if(!_agc)
	{
		LOGD("new Agc fail !\n");
		return;
	}
}

AgcImpl::~AgcImpl()
{
	if(_agc)
	{
		delete _agc;
		_agc = NULL;
	}
}

int AgcImpl::deal(short *inData, short *outData)
{
	if(_agc)
	{
		return _agc->deal(inData, outData);
	}

	return -1;
}

