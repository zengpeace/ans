#include "ans.h"


Ans::Ans(const unsigned int sampleRate, const int mode)
{
	LOGD("+%s:%u %d\n", __FUNCTION__, sampleRate, mode);
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

	LOGD("-%s:\n", __FUNCTION__);
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


AnsAdjustLen::AnsAdjustLen(const unsigned int sampleRate, const int mode)
{
	LOGD("+%s:%u %d\n", __FUNCTION__, sampleRate, mode);
	_ans = new Ans(sampleRate, mode);
	if(!_ans)
	{
		LOGD("new Ans fail ! %u, %d\n", sampleRate, mode);
		return;
	}

	LOGD("new Ans success !\n");
	_sampleNum = sampleRate / 1000;
	memset(_finishBuf, 0, BUF_SIZE * 2);
	memset(_unfinishBuf, 0, BUF_SIZE * 2);
	_finishSize = 0;
	_unfinishSize = 0;

	LOGD("-%s:\n", __FUNCTION__);
}

AnsAdjustLen::~AnsAdjustLen()
{
	if(_ans)
	{
		delete _ans;
		_ans = NULL;
	}
}

int AnsAdjustLen::deal(short *data, const unsigned int bytes, short *outData)
{
	memcpy((unsigned char*)_unfinishBuf + _unfinishSize*2, (unsigned char*)data, bytes);
    _unfinishSize += bytes / 2;

    short* inData = _unfinishBuf;
    int restInSize = _unfinishSize;
    while(restInSize >= (int)_sampleNum)
    {
        _ans->deal(inData, NULL, _finishBuf + _finishSize, NULL);
        _finishSize += _sampleNum;
        restInSize -= _sampleNum;
        inData += _sampleNum;
    }

	_unfinishSize = restInSize;
    if(_unfinishSize > 0)
    {
        memcpy((unsigned char*)_unfinishBuf, (unsigned char*)inData, _unfinishSize * 2);
    }

    if(_finishSize < (int)bytes / 2)
    {
        LOGD("not enough data ! %d, %d\n", _finishSize * 2, bytes);
        return -1;
    }

    memcpy(outData?outData:data, (unsigned char*)_finishBuf, bytes);

	_finishSize -= bytes / 2;
    memcpy((unsigned char*)_finishBuf, (unsigned char*)_finishBuf + bytes, _finishSize * 2);
	return 0;
}


AnsImpl::AnsImpl(const unsigned int sampleRate, const int mode)
{
	_interleaveBuf = NULL;
	_left = NULL;
	_right = NULL;

	_interleaveBuf = new short*[2];
	if(!_interleaveBuf)
	{
		LOGD("new buf fail !\n");
		return;
	}
	
	for(int i = 0; i < 2; i ++)
	{
		_interleaveBuf[i] = new short[BUF_SIZE];
		if(!_interleaveBuf[i])
		{
			LOGD("new fail !\n");
			return;
		}
	}

	LOGD("finish _interleaveBuf new !\n");

	_left = new AnsAdjustLen(sampleRate, mode);
	_right = new AnsAdjustLen(sampleRate, mode);

	if(!_left || !_right)
	{
		LOGD("new AnsAdjustLen fail ! %p, %p\n", _left, _right);
		return;
	}

	_fpIn = fopen("/srv/www/upload/in.pcm", "wb");
	_fpOut = fopen("/srv/www/upload/out.pcm", "wb");

	LOGD("new AnsImpl success !\n");
}

AnsImpl::~AnsImpl()
{
	if(_left)
	{
		delete _left;
		_left = NULL;
	}

	if(_right)
	{
		delete _right;
		_right = NULL;
	}

	if(_fpIn)
	{
		fclose(_fpIn);
		_fpIn = NULL;
	}

	if(_fpOut)
	{
		fclose(_fpOut);
		_fpOut = NULL;
	}
}

/*int AnsImpl::deal(short *data, const unsigned int bytes)
{
	int ret;
	unsigned int perChannelSampleNum, interleaveSize;
	deInterleave(data, bytes/2, _interleaveBuf, perChannelSampleNum);
	ret = _left->deal(_interleaveBuf[0], perChannelSampleNum * 2);
	ret = _right->deal(_interleaveBuf[1], perChannelSampleNum * 2);
	interleave(_interleaveBuf, perChannelSampleNum, data, interleaveSize);

	return ret;
}*/

int AnsImpl::deal(short *data, const unsigned int bytes)
{
	for(unsigned int i = 0; i < bytes / 4; i ++)
	{
		_interleaveBuf[0][i] = data[2 * i];
	}

	fwrite(_interleaveBuf[0], 1, bytes / 2, _fpIn);
	int ret = _left->deal(_interleaveBuf[0], bytes / 4, data);
	fwrite(data, 1, bytes / 2, _fpOut);
	return ret;
}

void AnsImpl::interleave(short** inData, const unsigned int inSize, short* outData, unsigned int& outSize)
{
	//LOGD("%s: inSize=%u\n", __FUNCTION__, inSize);
	for(unsigned int i = 0; i < inSize; i ++)
	{
		outData[2 * i] = inData[0][i];			
		outData[2 * i + 1] = inData[1][i];
	}

	outSize = inSize * 2;
	//LOGD("%s: outSize=%u\n", __FUNCTION__, outSize);
}

void AnsImpl::deInterleave(short* inData, const unsigned int inSize, short** outData, unsigned int& outSize)
{
	//LOGD("%s: inSize=%u\n", __FUNCTION__, inSize);
	for(unsigned int i = 0; i < inSize; i ++)
	{
		outData[0][i] = inData[i * 2];
		outData[1][i] = inData[i * 2 + 1];
	}

	outSize = inSize / 2;
	//LOGD("%s: outSize=%u\n", __FUNCTION__, outSize);
}




