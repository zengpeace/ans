#ifndef _ANS_H_
#define _ANS_H_

#include <iostream>
#include <time.h>
#include "stdio.h"
#include "noise_suppression_x.h"

#define LOGD printf

class Ans
{
public:
	Ans(const unsigned int sampleRate, const int mode);
	~Ans();

	int deal(short *speechFrame, short *speechFrameHB, short *outFrame, short *outFrameHB);

private:
	NsxHandle* _handle;
};

#endif

