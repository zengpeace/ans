#ifndef _ANS_H_
#define _ANS_H_

#include <iostream>
#include <time.h>
#include "stdio.h"
#include "noise_suppression_x.h"

#include "agc.h"

#include "common_head.h"

#ifndef LOGD
#define LOGD udi_erro_log
#endif

class Ans
{
public:
	Ans(const unsigned int sampleRate, const int mode);
	~Ans();

	int deal(short *speechFrame, short *speechFrameHB, short *outFrame, short *outFrameHB);

private:
	Ans();
	NsxHandle* _handle;
};


class AnsAdjustLen
{
public:
#define BUF_SIZE (4096 * 2)

	AnsAdjustLen(const unsigned int sampleRate, const int mode);
	~AnsAdjustLen();

	int deal(short *data, const unsigned int bytes, short *outData = NULL);

private:
	AnsAdjustLen();

	Ans *_ans;
	AgcImpl *_agcImpl;
	unsigned int _sampleNum;
	short _finishBuf[BUF_SIZE];
	short _unfinishBuf[BUF_SIZE];
	int _finishSize;
	int _unfinishSize;

	short _agcTmpBuf[BUF_SIZE];
};


class AnsImpl
{
public:
#define BUF_SIZe (4096 * 2)
	AnsImpl(const unsigned int sampleRate, const int mode);
	~AnsImpl();

	int deal(short *data, const unsigned int bytes);

private:
	AnsImpl();
	void interleave(short** inData, const unsigned int inSize, short* outData, unsigned int& outSize);
	void deInterleave(short* inData, const unsigned int inSize, short** outData, unsigned int& outSize);

	AnsAdjustLen *_left;
	AnsAdjustLen *_right;	
	short **_interleaveBuf;

	FILE *_fpIn, *_fpOut;
};

#endif

