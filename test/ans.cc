#include "ans.h"


Ans::Ans(const unsigned int sampleRate, const int mode)
{
	_handle = NULL;

	char buf[256];
	WebRtcNsx_get_version(buf, 256);
	LOGD("nsx version is %s\n", buf);

	int ret;
	ret = WebRtcNsx_Create(&_handle);
	if (ret < 0)
	{
		LOGD("WebRtcNsx_Create fail ! ret = %d\n", ret);
		_handle = NULL;
		return;
	}

	ret = WebRtcNsx_Init(_handle, sampleRate);
	if (ret < 0)
	{
		LOGD("WebRtcNsx_Init fail ! ret = %d\n", ret);
		_handle = NULL;
		return;
	}

	ret = WebRtcNsx_set_policy(_handle, mode);
	if (ret < 0)
	{
		LOGD("WebRtcNsx_Init fail ! ret = %d\n", ret);
		//_handle = NULL;
		return;
	}
}

Ans::~Ans()
{
	if (_handle)
	{
		WebRtcNsx_Free(_handle);
		_handle = NULL;
	}
}

int Ans::deal(short *speechFrame, short *speechFrameHB, short *outFrame, short *outFrameHB)
{
	return WebRtcNsx_Process(_handle, speechFrame, speechFrameHB, outFrame, outFrameHB);
}
